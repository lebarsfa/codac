/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <codac2_Color.h>

using namespace std;
using namespace codac2;

// Custom Matchers to manage tolerate errors

// array<int,4> matcher

class Array4MatcherWithTolerance : public Catch::Matchers::MatcherBase<std::array<int, 4>> {
    std::array<int, 4> m_expected;
    int m_tolerance;

public:
    Array4MatcherWithTolerance(const std::array<int, 4>& expected, int tolerance)
        : m_expected(expected), m_tolerance(tolerance) {}

    bool match(const std::array<int, 4>& actual) const override {
        for (size_t i = 0; i < m_expected.size(); ++i) {
            if (std::abs(actual[i] - m_expected[i]) > m_tolerance) {
                return false;
            }
        }
        return true;
    }

    std::string describe() const override {
        std::ostringstream oss;
        oss << "is close to { ";
        for (size_t i = 0; i < m_expected.size(); ++i) {
            oss << m_expected[i];
            if (i < m_expected.size() - 1) oss << ", ";
        }
        oss << " } with tolerance " << m_tolerance;
        return oss.str();
    }
};

// array<int,3> matcher

class Array3MatcherWithTolerance : public Catch::Matchers::MatcherBase<std::array<int, 3>> {
    std::array<int, 3> m_expected;
    int m_tolerance;

public:
    Array3MatcherWithTolerance(const std::array<int, 3>& expected, int tolerance)
        : m_expected(expected), m_tolerance(tolerance) {}

    bool match(const std::array<int, 3>& actual) const override {
        for (size_t i = 0; i < m_expected.size(); ++i) {
            if (std::abs(actual[i] - m_expected[i]) > m_tolerance) {
                return false;
            }
        }
        return true;
    }

    std::string describe() const override {
        std::ostringstream oss;
        oss << "is close to { ";
        for (size_t i = 0; i < m_expected.size(); ++i) {
            oss << m_expected[i];
            if (i < m_expected.size() - 1) oss << ", ";
        }
        oss << " } with tolerance " << m_tolerance;
        return oss.str();
    }
};

// HTML color matcher

class HtmlColorMatcherWithTolerance : public Catch::Matchers::MatcherBase<std::string> {
    std::string m_expected;
    int m_tolerance;

    // Helper to convert a hex color to RGB
    static std::array<int, 3> hexToRgb(const std::string& hex) {
        assert(hex.size() == 7 && hex[0] == '#'); // Ensure format is #RRGGBB
        return {
            std::stoi(hex.substr(1, 2), nullptr, 16),
            std::stoi(hex.substr(3, 2), nullptr, 16),
            std::stoi(hex.substr(5, 2), nullptr, 16)
        };
    }

public:
    HtmlColorMatcherWithTolerance(const std::string& expected, int tolerance)
        : m_expected(expected), m_tolerance(tolerance) {
        assert(m_expected.size() == 7 && m_expected[0] == '#'); // Ensure format is #RRGGBB
    }

    bool match(const std::string& actual) const override {
        if (actual.size() != 7 || actual[0] != '#') {
            return false; // Invalid format
        }

        auto expectedRgb = hexToRgb(m_expected);
        auto actualRgb = hexToRgb(actual);

        for (size_t i = 0; i < 3; ++i) {
            if (std::abs(actualRgb[i] - expectedRgb[i]) > m_tolerance) {
                return false;
            }
        }
        return true;
    }

    std::string describe() const override {
        std::ostringstream oss;
        oss << "is close to " << m_expected << " with tolerance " << m_tolerance;
        return oss.str();
    }
};

// RGBA HTML color matcher

class HtmlColorMatcherWithToleranceRGBA : public Catch::Matchers::MatcherBase<std::string> {
    std::string m_expected;
    int m_tolerance;

    // Helper to convert a hex color to RGBA
    static std::array<int, 4> hexToRgba(const std::string& hex) {
        assert(hex.size() == 9 && hex[0] == '#'); // Ensure format is #RRGGBBAA
        return {
            std::stoi(hex.substr(1, 2), nullptr, 16),
            std::stoi(hex.substr(3, 2), nullptr, 16),
            std::stoi(hex.substr(5, 2), nullptr, 16),
            std::stoi(hex.substr(7, 2), nullptr, 16)
        };
    }

public:
    HtmlColorMatcherWithToleranceRGBA(const std::string& expected, int tolerance)
        : m_expected(expected), m_tolerance(tolerance) {
        assert(m_expected.size() == 9 && m_expected[0] == '#'); // Ensure format is #RRGGBBAA
    }

    bool match(const std::string& actual) const override {
        if (actual.size() != 9 || actual[0] != '#') {
            return false; // Invalid format
        }

        auto expectedRgba = hexToRgba(m_expected);
        auto actualRgba = hexToRgba(actual);

        for (size_t i = 0; i < 4; ++i) {
            if (std::abs(actualRgba[i] - expectedRgba[i]) > m_tolerance) {
                return false;
            }
        }
        return true;
    }

    std::string describe() const override {
        std::ostringstream oss;
        oss << "is close to " << m_expected << " with tolerance " << m_tolerance;
        return oss.str();
    }
};

inline Array4MatcherWithTolerance IsCloseTo(const std::array<int, 4>& expected, int tolerance = 1) {
    return Array4MatcherWithTolerance(expected, tolerance);
}

inline Array3MatcherWithTolerance IsCloseTo(const std::array<int, 3>& expected, int tolerance = 1) {
    return Array3MatcherWithTolerance(expected, tolerance);
}

inline HtmlColorMatcherWithTolerance IsCloseToHtmlColor(const std::string& expected, int tolerance = 1) {
    return HtmlColorMatcherWithTolerance(expected, tolerance);
}

inline HtmlColorMatcherWithToleranceRGBA IsCloseToHtmlColorRGBA(const std::string& expected, int tolerance = 1) {
    return HtmlColorMatcherWithToleranceRGBA(expected, tolerance);
}


TEST_CASE("Color")
{
  {
    // Red

    std::array<int,3> d_rgb  { 255,0,0 };
    std::array<int,4> d_rgba { 255,0,0,255 };
    std::array<int,3> d_hsv  { 0,100,100 };
    std::array<int,4> d_hsva { 0,100,100,100 };
    std::array<float,3> d_rgb_f  { 1.0,0.0,0.0 };
    std::array<float,4> d_rgba_f { 1.0,0.0,0.0,1.0 };
    std::array<float,3> d_hsv_f  { 0.0,1.0,1.0 };
    std::array<float,4> d_hsva_f { 0.0,1.0,1.0,1.0 };

    vector<Color> v {
      Color(d_rgb, InterpolMode::RGB /* InterpolMode::RGB is default */),
      Color(d_rgba, InterpolMode::RGB),
      Color(255,0,0, InterpolMode::RGB),
      Color(255,0,0,255, InterpolMode::RGB),
      Color(d_hsv, InterpolMode::HSV),
      Color(d_hsva, InterpolMode::HSV),
      Color(0,100,100, InterpolMode::HSV),
      Color(0,100,100,100, InterpolMode::HSV),
      Color(d_rgb_f, InterpolMode::RGB),
      Color(d_rgba_f, InterpolMode::RGB),
      Color( 1.0, 0.0, 0.0, InterpolMode::RGB),
      Color( 1.0, 0.0, 0.0, 1.0, InterpolMode::RGB),
      Color(d_hsv_f, InterpolMode::HSV),
      Color(d_hsva_f, InterpolMode::HSV),
      Color( 0.0, 1.0, 1.0, InterpolMode::HSV),
      Color( 0.0, 1.0, 1.0, 1.0, InterpolMode::HSV),
      Color("#FF0000")
    };

    for(const auto& c : v)
    {
      CHECK_THAT(c.hex_str(), IsCloseToHtmlColor("#ff0000"));
      CHECK_THAT(c.rgb(), IsCloseTo(std::array<int,3>{ 255,0,0})); // return std::array<int,3>
      CHECK_THAT(c.rgba(), IsCloseTo(std::array<int,4>{ 255,0,0,255})); // return std::array<int,4>
      CHECK_THAT(c.hsv(), IsCloseTo(std::array<int,3>{ 0,100,100})); // return std::array<int,3>
      CHECK_THAT(c.hsva(), IsCloseTo(std::array<int,4>{ 0,100,100,100})); // return std::array<int,4>
    }
  }

  {
    // Pink full opacity

    int a = 255;
    std::array<int,3> d_rgb  { 229,128,255 };
    std::array<int,4> d_rgba { 229,128,255,a };
    std::array<int,3> d_hsv  { 288,50,100 };
    std::array<int,4> d_hsva { 288,50,100,100};
    std::array<float,3> d_rgb_f  {(float) (229./255.), (float) (128./255.), (float) (255./255.)};
    std::array<float,4> d_rgba_f {(float) (229./255.), (float) (128./255.), (float) (255./255.), 1.0};
    std::array<float,3> d_hsv_f  {(float) (288./360.), (float) (50./100.), (float) (100./100.)};
    std::array<float,4> d_hsva_f {(float) (288./360.), (float) (50./100.), (float) (100./100.), 1.0};
    

    vector<Color> v {
      Color(d_rgb, InterpolMode::RGB /* InterpolMode::RGB is default */),
      Color(d_rgba, InterpolMode::RGB),
      Color(229,128,255, InterpolMode::RGB),
      Color(229,128,255,a, InterpolMode::RGB),
      Color(d_hsv, InterpolMode::HSV),
      Color(d_hsva, InterpolMode::HSV),
      Color(288,50,100, InterpolMode::HSV),
      Color(288,50,100,100, InterpolMode::HSV),
      Color(d_rgb_f, InterpolMode::RGB),
      Color(d_rgba_f, InterpolMode::RGB),
      Color( 229./255., 128./255., 255./255., InterpolMode::RGB),
      Color( 229./255., 128./255., 255./255., 1.0, InterpolMode::RGB),
      Color(d_hsv_f, InterpolMode::HSV),
      Color(d_hsva_f, InterpolMode::HSV),
      Color( 288./360., 50./100., 100./100., InterpolMode::HSV),
      Color( 288./360.,50./100., 100./100., 1.0, InterpolMode::HSV),
      Color("#e580ff")
    };

    for(const auto& c : v)
    {
      CHECK_THAT(c.hex_str(), IsCloseToHtmlColor("#e580ff"));
      CHECK_THAT(c.rgb(), IsCloseTo(std::array<int,3>{ 229,128,255})); // return std::array<int,3>
      CHECK_THAT(c.rgba(), IsCloseTo(std::array<int,4>{ 229,128,255,a})); // return std::array<int,4>
      CHECK_THAT(c.hsv(), IsCloseTo(std::array<int,3>{ 288,50,100})); // return std::array<int,3>
      CHECK_THAT(c.hsva(), IsCloseTo(std::array<int,4>{ 288,50,100,100})); // return std::array<int,4>
    }
  }

  {
    // Pink 40% opacity

    int a = 0.4*255;
    std::array<int,4> d_rgba { 229,128,255,a };
    std::array<int,4> d_hsva { 288,50,100,40 };
    std::array<float,4> d_rgba_f {(float) (229./255.), (float) (128./255.), (float) (255./255.),(float) 0.4};
    std::array<float,4> d_hsva_f {(float) (288./360.), (float) (50./100.), (float) (100./100.), (float) 0.4};

    vector<Color> v {
      Color(d_rgba, InterpolMode::RGB),
      Color(229,128,255,a, InterpolMode::RGB),
      Color(d_hsva, InterpolMode::HSV),
      Color(288,50,100,40, InterpolMode::HSV),
      Color(d_rgba_f, InterpolMode::RGB),
      Color( 229./255., 128./255., 255./255., 0.4, InterpolMode::RGB),
      Color(d_hsva_f, InterpolMode::HSV),
      Color( 288./360.,50./100., 100./100., 0.4, InterpolMode::HSV),
      Color("#e580ff66")
    };

    for(const auto& c : v)
    {
      CHECK_THAT(c.hex_str(), IsCloseToHtmlColorRGBA("#e580ff66"));
      CHECK_THAT(c.rgb(), IsCloseTo(std::array<int,3>{ 229,128,255})); // return std::array<int,3>
      CHECK_THAT(c.rgba(), IsCloseTo(std::array<int,4>{ 229,128,255,a})); // return std::array<int,4>
      CHECK_THAT(c.hsv(), IsCloseTo(std::array<int,3>{ 288,50,100})); // return std::array<int,3>
      CHECK_THAT(c.hsva(), IsCloseTo(std::array<int,4>{ 288,50,100,40})); // return std::array<int,4>
    }
  }
}