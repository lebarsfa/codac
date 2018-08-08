/* ============================================================================
 *  tubex-lib - TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeVector.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TrajectoryVectorSerialization.h"
#include "tubex_DimensionException.h"
#include "tubex_SlicingException.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_Arithmetic.h"
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeVector::TubeVector()
    {
      
    }

    TubeVector::TubeVector(const Interval& domain, int dim)
      : TubeVector(domain, IntervalVector(dim))
    {
      DomainException::check(domain);
      DimensionException::check(dim);
    }

    TubeVector::TubeVector(const Interval& domain, const IntervalVector& codomain)
    {
      DomainException::check(domain);
      DimensionException::check(codomain.size());

      // By default, the tube is defined as one single slice
      m_first_slice = new Slice(domain, codomain);
      m_first_slice->set_tube_ref(this);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, int dim)
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      DimensionException::check(dim);

      Slice *prev_slice = NULL, *slice;
      double lb, ub = domain.lb();

      if(timestep == 0.)
        timestep = domain.diam();

      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = min(lb + timestep, domain.ub());

        slice = new Slice(Interval(lb,ub), dim);
        slice->set_tube_ref(this);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          Slice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
        if(m_first_slice == NULL) m_first_slice = slice;
        slice = slice->next_slice();

      } while(ub < domain.ub());
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const IntervalVector& codomain)
      : TubeVector(domain, timestep, codomain.size())
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const tubex::Fnc& f)
      : TubeVector(domain, timestep, f.image_dim())
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      if(f.nb_vars() != 0)
        throw Exception("TubeVector constructor",
                        "function's inputs not limited to system variable");
      // A copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(*this, IntervalVector(1)); // 1: one variable (system variable t)
      *this = f.eval(input);
    }

    TubeVector::TubeVector(const TubeVector& x)
    {
      *this = x;
    }

    TubeVector::TubeVector(const TubeVector& x, const IntervalVector& codomain)
      : TubeVector(x)
    {
      resize(codomain.size());
      set(codomain);
    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
      : TubeVector(traj.domain(), timestep, traj.dim())
    {
      DomainException::check(timestep);
      set_empty();
      *this |= traj;
    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
      : TubeVector(lb.domain(), timestep, lb.dim())
    {
      DomainException::check(timestep);
      DimensionException::check(lb, ub);
      set_empty();
      *this |= lb;
      *this |= ub;
    }

    TubeVector::TubeVector(const string& binary_file_name)
    {
      vector<TrajectoryVector> v_trajs;
      deserialize(binary_file_name, v_trajs);
    }
    
    TubeVector::TubeVector(const string& binary_file_name, TrajectoryVector& traj)
    {
      vector<TrajectoryVector> v_trajs;
      deserialize(binary_file_name, v_trajs);

      if(v_trajs.size() == 0)
        throw Exception("TubeVector constructor", "unable to deserialize a TrajectoryVector");

      traj = v_trajs[0];
    }

    TubeVector::TubeVector(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      deserialize(binary_file_name, v_trajs);
      if(v_trajs.size() == 0)
        throw Exception("TubeVector constructor", "unable to deserialize some TrajectoryVector");
    }
    
    TubeVector::~TubeVector()
    {
      Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        Slice *next_slice = slice->next_slice();
        delete slice;
        slice = next_slice;
      }
    }

    const TubeVector TubeVector::primitive() const
    {
      TubeVector primitive(*this, IntervalVector(dim(), Interval::ALL_REALS));
      primitive.set(IntervalVector(dim(), 0.), primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, static_cast<const TubeVector&>(*this), FORWARD);
      return primitive;
    }

    const TubeVector& TubeVector::operator=(const TubeVector& x)
    {
      { // Destroying already existing slices
        Slice *slice = get_first_slice();
        while(slice != NULL)
        {
          Slice *next_slice = slice->next_slice();
          delete slice;
          slice = next_slice;
        }
      }

      Slice *prev_slice = NULL, *slice = NULL;

      for(const Slice *s = x.get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(slice == NULL)
        {
          slice = new Slice(*s);
          m_first_slice = slice;
        }

        else
        {
          slice->m_next_slice = new Slice(*s);
          slice = slice->next_slice();
        }

        slice->set_tube_ref(this);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          Slice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
      }

      return *this;
    }

    const Interval TubeVector::domain() const
    {
      return Interval(get_first_slice()->domain().lb(),
                      get_last_slice()->domain().ub());
    }

    int TubeVector::dim() const
    {
      return get_first_slice()->dim();
    }

    void TubeVector::resize(int n)
    {
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->resize(n);
    }
    
    const TubeVector TubeVector::subvector(int start_index, int end_index) const
    {
      // todo; check start_index, end_index, <
      TubeVector subvec(domain(), end_index - start_index + 1);
      Slice *ssv = subvec.get_first_slice();
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        ssv->set_envelope(s->codomain().subvector(start_index, end_index));
        ssv->set_input_gate(s->input_gate().subvector(start_index, end_index));
        ssv->set_output_gate(s->output_gate().subvector(start_index, end_index));
        ssv = ssv->next_slice();
      }
      return subvec;
    }
    
    void TubeVector::put(int start_index, const TubeVector& subvec)
    {
      // todo: check size subvec
      // todo: check structure tubes

      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set_all_reals(start_index, start_index + subvec.dim() - 1);

      const Slice *ssv = subvec.get_first_slice();
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        IntervalVector codomain = s->codomain();
        IntervalVector input_gate = s->input_gate();

        ibex_overloaded_put(codomain, start_index, ssv->codomain());
        s->set_envelope(codomain);
        ibex_overloaded_put(input_gate, start_index, ssv->input_gate());
        s->set_input_gate(input_gate);

        ssv = ssv->next_slice();
      }

      Slice *s = get_last_slice();
      IntervalVector output_gate = s->output_gate();
      ibex_overloaded_put(output_gate, start_index, subvec.get_last_slice()->output_gate());
      s->set_output_gate(output_gate);
    }
  
    // Slices structure

    int TubeVector::nb_slices() const
    {
      int size = 0;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        size ++;
      return size;
    }

    Slice* TubeVector::get_slice(int slice_id)
    {
      SlicingException::check(*this, slice_id);
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_slice(slice_id));
    }

    const Slice* TubeVector::get_slice(int slice_id) const
    {
      SlicingException::check(*this, slice_id);
      int i = 0;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(i == slice_id)
          return s;
        i++;
      }
      return NULL;
    }

    Slice* TubeVector::get_slice(double t)
    {
      DomainException::check(*this, t);
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_slice(t));
    }

    const Slice* TubeVector::get_slice(double t) const
    {
      DomainException::check(*this, t);
      return get_slice(input2index(t));
    }

    Slice* TubeVector::get_first_slice()
    {
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_first_slice());
    }

    const Slice* TubeVector::get_first_slice() const
    {
      return m_first_slice;
    }

    Slice* TubeVector::get_last_slice()
    {
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_last_slice());
    }

    const Slice* TubeVector::get_last_slice() const
    {
      for(const Slice *s = get_first_slice() ; true ; s = s->next_slice())
        if(s->next_slice() == NULL)
          return s;
      return NULL;
    }

    Slice* TubeVector::get_wider_slice()
    {
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_wider_slice());
    }

    const Slice* TubeVector::get_wider_slice() const
    {
      double max_domain_width = 0.;
      const Slice *wider_slice;

      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        if(s->domain().diam() > max_domain_width)
        {
          wider_slice = s;
          max_domain_width = s->domain().diam();
        }

      return wider_slice;
    }

    Slice* TubeVector::get_largest_slice()
    {
      return const_cast<Slice*>(static_cast<const TubeVector&>(*this).get_largest_slice());
    }

    const Slice* TubeVector::get_largest_slice() const
    {
      int i = 0;
      double max_diam = 0.;
      const Slice *largest = get_first_slice();
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s->codomain().is_unbounded())
          return s;

        if(s->codomain().max_diam() > max_diam)
        {
          max_diam = s->codomain().max_diam();
          largest = s;
        }
      }

      return largest;
    }

    int TubeVector::input2index(double t) const
    {
      DomainException::check(*this, t);

      int i = -1;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        i++;
        if(t < s->domain().ub())
          return i;
      }

      return i;
    }

    int TubeVector::index(const Slice* slice) const
    {
      int i = 0;
      const Slice *it = get_first_slice();
      while(it != NULL && it != slice)
      {
        it = it->next_slice();
        if(it == NULL) return -1;
        i++;
      }
      return i;
    }

    void TubeVector::sample(double t)
    {
      DomainException::check(*this, t);

      Slice *slice_to_be_sampled = get_slice(t);

      if(slice_to_be_sampled->domain().lb() == t || slice_to_be_sampled->domain().ub() == t)
      {
        // No degenerate slice,
        // the method has no effect.
        return;
      }

      Slice *next_slice = slice_to_be_sampled->next_slice();

      // Creating new slice
      Slice *new_slice = new Slice(*slice_to_be_sampled);
      new_slice->set_tube_ref(this);
      new_slice->set_domain(Interval(t, slice_to_be_sampled->domain().ub()));
      slice_to_be_sampled->set_domain(Interval(slice_to_be_sampled->domain().lb(), t));

      // todo: remove this? vector<Slice*>::iterator it = find(m_v_slices.begin(), m_v_slices.end(), slice_to_be_sampled);
      // todo: remove this? m_v_slices.insert(++it, new_slice);

      // Updated slices structure
      delete new_slice->m_input_gate;
      new_slice->m_input_gate = NULL;
      Slice::chain_slices(new_slice, next_slice);
      Slice::chain_slices(slice_to_be_sampled, new_slice);
      new_slice->set_input_gate(new_slice->codomain());
    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);
      sample(t);
      Slice *slice = get_slice(t);
      if(t == slice->domain().lb())
        slice->set_input_gate(gate);
      else
        slice->set_output_gate(gate);
    }

    /*bool TubeVector::share_same_slicing(const TubeVector& x1, const TubeVector& x2)
    {
      DomainException::check(x1, x2);

      if(x1.nb_slices() != x2.nb_slices())
        return false;

      const Slice *s2 = x2.get_first_slice();
      for(const Slice *s1 = x1.get_first_slice() ; s1 != NULL ; s1 = s1->next_slice())
      {
        if(s1->domain() != s2->domain())
          return false;
        s2 = s2->next_slice();
      }

      return true;
    }*/

    // Access values

    const IntervalVector TubeVector::codomain() const
    {
      return codomain_box();
    }

    double TubeVector::volume() const
    {
      double volume = 0.;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        volume += s->volume();
      return volume;
    }

    TubeVectorComponent& TubeVector::operator[](int index)
    {
      return const_cast<TubeVectorComponent&>(static_cast<const TubeVector&>(*this)[index]);
    }

    const TubeVectorComponent& TubeVector::operator[](int index) const
    {
      // todo: check index
      if(m_m_tube_components.find(index) == m_m_tube_components.end()) // if not found
      {
        cout << "ok" << endl;
        m_m_tube_components[index] = TubeVectorComponent(const_cast<TubeVector*>(this), index);
      }
cout << index << endl;
      return m_m_tube_components[index];
    }

    const IntervalVector TubeVector::operator()(int slice_id) const
    {
      SlicingException::check(*this, slice_id);
      return get_slice(slice_id)->codomain();
    }

    const IntervalVector TubeVector::operator()(double t) const
    {
      DomainException::check(*this, t);
      return get_slice(t)->operator()(t);
    }

    const IntervalVector TubeVector::operator()(const Interval& t) const
    {
      DomainException::check(*this, t);

      if(t.is_degenerated())
        return operator()(t.lb());

      const Slice *slice = get_slice(t.lb());
      const Slice *last_slice = get_slice(t.ub());
      if(last_slice->domain().lb() != t.ub())
        last_slice = last_slice->next_slice();

      IntervalVector codomain(dim(), Interval::EMPTY_SET);

      while(slice != NULL && slice != last_slice)
      {
        codomain |= slice->codomain();
        slice = slice->next_slice();
      }

      return codomain;
    }

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeVector v(*this, Interval::ALL_REALS); // todo: optimize this
      return invert(y, v, search_domain);
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeVector v(*this, Interval::ALL_REALS); // todo: optimize this
      v_t.clear();
      invert(y, v_t, v, search_domain);
    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, v);
      DomainException::check(*this, v);

      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return Interval::EMPTY_SET;

      const Slice *slice_x = get_slice(intersection.lb());
      const Slice *slice_xdot = v.get_slice(intersection.lb());
      while(slice_x != NULL && slice_x->domain().lb() < intersection.ub())
      {
        if(slice_x->codomain().intersects(y))
          invert |= slice_x->invert(y, *slice_xdot, intersection);

        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      return invert;
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      v_t.clear();

      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return;

      const Slice *slice_x = get_slice(intersection.lb());
      const Slice *slice_xdot = v.get_slice(intersection.lb());
      while(slice_x != NULL && slice_x->domain().lb() <= intersection.ub())
      {
        Interval local_invert = slice_x->invert(y, *slice_xdot, intersection);
        if(local_invert.is_empty() && !invert.is_empty())
        {
          v_t.push_back(invert);
          invert.set_empty();
        }

        else
          invert |= local_invert;

        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> enclosed_bounds
        = make_pair(IntervalVector(dim(), Interval::EMPTY_SET), IntervalVector(dim(), Interval::EMPTY_SET));

      Interval intersection = t & domain();
      if(t.is_empty())
        return enclosed_bounds;

      const Slice *slice = get_slice(intersection.lb());
      while(slice != NULL && slice->domain().lb() <= intersection.ub())
      {
        pair<IntervalVector,IntervalVector> local_eval = slice->eval(intersection);
        enclosed_bounds.first |= local_eval.first;
        enclosed_bounds.second |= local_eval.second;
        slice = slice->next_slice();
      }

      return enclosed_bounds;
    }

    const IntervalVector TubeVector::interpol(double t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

      const Slice *slice_x = get_slice(t);
      if(slice_x->domain().lb() == t || slice_x->domain().ub() == t)
        return (*slice_x)(t);

      return interpol(Interval(t), v);
      // todo: check a faster implementation for this degenerate case?
    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

      IntervalVector interpol(dim(), Interval::EMPTY_SET);

      const Slice *slice_x = get_slice(t.lb());
      const Slice *slice_xdot = v.get_slice(t.lb());
      while(slice_x != NULL && slice_x->domain().lb() < t.ub())
      {
        interpol |= slice_x->interpol(t & slice_x->domain(), *slice_xdot);
        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      return interpol;
    }

    const Vector TubeVector::max_thickness() const
    {
      const Slice *largest = get_largest_slice();

      if(largest->codomain().is_unbounded())
        return Vector(dim(), numeric_limits<double>::infinity());

      else
        return largest->codomain().diam();
    }

    const Vector TubeVector::max_gate_thickness(double& t) const
    {
      const Slice *slice = get_first_slice();

      if(slice->input_gate().is_unbounded())
      {
        t = slice->domain().lb();
        return Vector(dim(), numeric_limits<double>::infinity());
      }

      double max_diam = 0.;
      Vector max_thickness = slice->input_gate().diam();

      while(slice != NULL)
      {
        if(slice->output_gate().is_unbounded())
        {
          t = slice->domain().ub();
          return Vector(dim(), numeric_limits<double>::infinity());
        }

        if(slice->output_gate().max_diam() > max_diam)
        {
          max_diam = slice->codomain().max_diam();
          max_thickness = slice->output_gate().diam();
          t = slice->domain().ub();
        }

        slice = slice->next_slice();
      }

      return max_thickness;
    }

    // Tests

    #define test_content(f, bool_output) \
      \
      DimensionException::check(*this, x); \
      SlicingException::check(*this, x); \
      const Slice *slice = get_first_slice(), *slice_x = x.get_first_slice(); \
      while(slice != NULL) \
      { \
        if(slice->f(*slice_x)) \
          return bool_output; \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return !bool_output;

    bool TubeVector::operator==(const TubeVector& x) const
    {
      if(x.nb_slices() != nb_slices())
        return false;
      test_content(operator!=, false);
    }

    bool TubeVector::operator!=(const TubeVector& x) const
    {
      if(x.nb_slices() != nb_slices())
        return true;
      test_content(operator!=, true);
    }

    bool TubeVector::is_subset(const TubeVector& x) const
    {
      test_content(is_subset, true);
    }

    bool TubeVector::is_strict_subset(const TubeVector& x) const
    {
      test_content(is_strict_subset, true);
    }

    bool TubeVector::is_empty() const
    {
      const Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        if(slice->is_empty()) return true;
        slice = slice->next_slice();
      }
      return false;
    }

    bool TubeVector::encloses(const TrajectoryVector& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);

      const Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        if(!slice->encloses(x)) return false;
        slice = slice->next_slice();
      }
      
      return true;
    }

    // Setting values

    void TubeVector::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set(y);
    }

    void TubeVector::set(const IntervalVector& y, int slice_id)
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, slice_id);
      get_slice(slice_id)->set(y);
    }

    void TubeVector::set(const IntervalVector& y, double t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);
      sample(t, y);
    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);

      if(t.is_degenerated())
        set(y, t.lb());

      else
      {
        sample(t.lb());
        sample(t.ub());

        Slice *slice = get_slice(input2index(t.lb()));
        while(slice != NULL && !(t & slice->domain()).is_degenerated())
        {
          slice->set(y);
          slice = slice->next_slice();
        }
      }
    }

    void TubeVector::set_empty()
    {
      IntervalVector empty_box(dim(), Interval::EMPTY_SET);
      set(empty_box);
    }

    const TubeVector& TubeVector::inflate(double rad)
    {
      IntervalVector e(dim(), Interval(-rad,rad));

      // Setting envelopes before gates' inflation
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set_envelope(s->codomain() + e);

      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s == get_first_slice())
          s->set_input_gate(s->input_gate() + e);
        s->set_output_gate(s->output_gate() + e);
      }

      return *this;
    }

    const TubeVector& TubeVector::inflate(const TrajectoryVector& rad)
    {
      DimensionException::check(*this, rad);
      DomainException::check(*this, rad);

      // Setting envelopes before gates' inflation
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set_envelope(s->codomain() + Interval(-1.,1.) * rad(s->domain()));

      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s == get_first_slice())
          s->set_input_gate(s->input_gate() + Interval(-1.,1.) * rad(s->domain().lb()));
        s->set_output_gate(s->output_gate() + Interval(-1.,1.) * rad(s->domain().ub()));
      }

      return *this;
    }

    // Bisection
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      DomainException::check(*this, t);
      pair<TubeVector,TubeVector> p = make_pair(*this,*this);

      LargestFirst bisector(0., ratio); // todo: bisect according to another rule than largest first?

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect((*this)(t));
        p.first.set(p_codomain.first, t);
        p.second.set(p_codomain.second, t);
      }

      catch(ibex::NoBisectableVariableException&)
      {
        throw Exception("TubeVector::bisect", "unable to bisect, degenerated slice (ibex::NoBisectableVariableException)");
      };

      return p;
    }

    // String
    
    ostream& operator<<(ostream& str, const TubeVector& x)
    {
      str << "TubeVector " << x.domain() << "↦" << x.codomain()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      DomainException::check(*this, t);
      return integral(Interval(t));
    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t);
      pair<IntervalVector,IntervalVector> partial_ti = partial_integral(t);

      if(partial_ti.first.is_empty() || partial_ti.second.is_empty())
        return IntervalVector(dim(), Interval::EMPTY_SET);

      else if(partial_ti.first.is_unbounded() || partial_ti.second.is_unbounded())
        return IntervalVector(dim(), Interval::ALL_REALS);

      else
        return IntervalVector(partial_ti.first.lb()) | partial_ti.second.ub();
    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);

      pair<IntervalVector,IntervalVector> integral_t1 = partial_integral(t1);
      pair<IntervalVector,IntervalVector> integral_t2 = partial_integral(t2);

      if(integral_t1.first.is_empty() || integral_t1.second.is_empty() ||
         integral_t2.first.is_empty() || integral_t2.second.is_empty())
      {
        return IntervalVector(dim(), Interval::EMPTY_SET);
      }

      else if(integral_t1.first.is_unbounded() || integral_t1.second.is_unbounded() ||
              integral_t2.first.is_unbounded() || integral_t2.second.is_unbounded())
      {
        return IntervalVector(dim(), Interval::ALL_REALS);
      }

      else
      {
        Vector lb = (integral_t2.first - integral_t1.first).lb();
        Vector ub = (integral_t2.second - integral_t1.second).ub();
        return IntervalVector(lb) | ub;
      }
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t);

      Interval intv_t;
      const Slice *slice = get_first_slice();
      pair<IntervalVector,IntervalVector> p_integ
        = make_pair(IntervalVector(dim(), 0.), IntervalVector(dim(), 0.)), p_integ_uncertain(p_integ);

      while(slice != NULL && slice->domain().lb() < t.ub())
      {
        if(slice->codomain().is_empty())
        {
          p_integ.first.set_empty();
          p_integ.second.set_empty();
          return p_integ;
        }

        if(slice->codomain().is_unbounded())
        {
          p_integ.first = IntervalVector(dim(), Interval::ALL_REALS);
          p_integ.second = IntervalVector(dim(), Interval::ALL_REALS);
          return p_integ;
        }

        // From t0 to tlb

          intv_t = slice->domain() & Interval(domain().lb(), t.lb());
          if(!intv_t.is_empty())
          {
            p_integ.first += intv_t.diam() * slice->codomain().lb();
            p_integ.second += intv_t.diam() * slice->codomain().ub();
            p_integ_uncertain = p_integ;
          }

        // From tlb to tub

          intv_t = slice->domain() & t;
          if(!intv_t.is_empty())
          {
            pair<IntervalVector,IntervalVector> p_integ_temp(p_integ_uncertain);
            p_integ_uncertain.first += Interval(0., intv_t.diam()) * slice->codomain().lb();
            p_integ_uncertain.second += Interval(0., intv_t.diam()) * slice->codomain().ub();
          
            p_integ.first |= p_integ_uncertain.first;
            p_integ.second |= p_integ_uncertain.second;

            p_integ_uncertain.first = p_integ_temp.first + intv_t.diam() * slice->codomain().lb();
            p_integ_uncertain.second = p_integ_temp.second + intv_t.diam() * slice->codomain().ub();
          }

        slice = slice->next_slice();
      }

      return p_integ;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      pair<IntervalVector,IntervalVector> integral_t1 = partial_integral(t1);
      pair<IntervalVector,IntervalVector> integral_t2 = partial_integral(t2);
      return make_pair((integral_t2.first - integral_t1.first),
                       (integral_t2.second - integral_t1.second));
    }

    // Contractors

    bool TubeVector::ctc_deriv(const TubeVector& v)
    {
      CtcDeriv ctc;
      return ctc.contract(*this, v);
    }

    bool TubeVector::ctc_eval(Interval& t, IntervalVector& z, TubeVector& w)
    {
      CtcEval ctc;
      return ctc.contract(t, z, *this, w);
    }
      
    // Serialization

    void TubeVector::serialize(const string& binary_file_name, int version_number) const
    {
      vector<TrajectoryVector> v_trajs;
      serialize(binary_file_name, v_trajs, version_number);
    }

    void TubeVector::serialize(const string& binary_file_name, const TrajectoryVector& traj, int version_number) const
    {
      vector<TrajectoryVector> v_trajs;
      v_trajs.push_back(traj);
      serialize(binary_file_name, v_trajs, version_number);
    }
    
    void TubeVector::serialize(const string& binary_file_name, const vector<TrajectoryVector>& v_trajs, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serialize_tubevector(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serialize_trajvector(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Access values

    const IntervalVector TubeVector::codomain_box() const
    {
      IntervalVector codomain(dim(), Interval::EMPTY_SET);
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        codomain |= s->codomain_box();
      return codomain;
    }

    // Integration

    // Serialization

    void TubeVector::deserialize(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserialize_tubevector(bin_file, *this);

      if(!bin_file.eof())
      {
        int nb_trajs;
        bin_file.read((char*)&nb_trajs, sizeof(int));
        for(int i = 0 ; i < nb_trajs ; i++)
        {
          TrajectoryVector traj;
          deserialize_trajvector(bin_file, traj);
          v_trajs.push_back(traj);
        }
      }

      bin_file.close();
    }
}