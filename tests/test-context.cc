/*
 * These tests are of limited usefulness.  In fact, you might even say that
 * they're not really tests at all.  But I felt that it would be useful to have
 * some basic usage of most functions just to verify that things compile and
 * work generally
 */

#include <cfloat>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>
using namespace boost::unit_test;
#include <cairomm/context.h>
#include <cairomm/scaledfont.h>

#define CREATE_CONTEXT(varname) \
  auto surf = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 10, 10); \
  auto cr = Cairo::Context::create(surf);

BOOST_AUTO_TEST_SUITE( Cairo_Context )

BOOST_AUTO_TEST_CASE(test_dashes)
{
  CREATE_CONTEXT(cr);
  std::valarray<double> dash_array(4);
  dash_array[0] = 0.1;
  dash_array[1] = 0.2;
  dash_array[2] = 0.04;
  dash_array[3] = 0.31;
  cr->set_dash(dash_array, 0.54);

  std::vector<double> get_array;
  double get_offset;
  cr->get_dash (get_array, get_offset);
  BOOST_CHECK_EQUAL (dash_array[0], get_array[0]);
  BOOST_CHECK_EQUAL (dash_array[1], get_array[1]);
  BOOST_CHECK_EQUAL (dash_array[2], get_array[2]);
  BOOST_CHECK_EQUAL (dash_array[3], get_array[3]);
  BOOST_CHECK_EQUAL (0.54, get_offset);

  std::vector<double> dash_vect(4);
  dash_vect[0] = 0.5;
  dash_vect[1] = 0.25;
  dash_vect[2] = 0.93;
  dash_vect[3] = 1.31;
  cr->set_dash(dash_vect, 0.4);

  cr->get_dash (get_array, get_offset);
  BOOST_CHECK_EQUAL (dash_vect[0], get_array[0]);
  BOOST_CHECK_EQUAL (dash_vect[1], get_array[1]);
  BOOST_CHECK_EQUAL (dash_vect[2], get_array[2]);
  BOOST_CHECK_EQUAL (dash_vect[3], get_array[3]);
  BOOST_CHECK_EQUAL (0.4, get_offset);

  cr->unset_dash ();
  cr->get_dash (get_array, get_offset);
  BOOST_CHECK (get_array.empty ());
}

BOOST_AUTO_TEST_CASE(test_save_restore)
{
  CREATE_CONTEXT(cr);
  cr->set_line_width (2.3);
  cr->save ();
  cr->set_line_width (4.0);
  BOOST_CHECK_EQUAL (4.0, cr->get_line_width ());
  cr->restore ();
  BOOST_CHECK_EQUAL (2.3, cr->get_line_width ());
}

BOOST_AUTO_TEST_CASE(test_operator)
{
  CREATE_CONTEXT(cr);
  cr->set_operator (Cairo::OPERATOR_ATOP);
  BOOST_CHECK_EQUAL (Cairo::OPERATOR_ATOP, cr->get_operator ());
  cr->set_operator (Cairo::OPERATOR_CLEAR);
  BOOST_CHECK_EQUAL (Cairo::OPERATOR_CLEAR, cr->get_operator ());
}

BOOST_AUTO_TEST_CASE(test_source)
{
  CREATE_CONTEXT(cr);
  auto solid_pattern =
    Cairo::SolidPattern::create_rgb (1.0, 0.5, 0.25);
  auto gradient_pattern =
    Cairo::LinearGradient::create (0.0, 0.0, 1.0, 1.0);
  auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 1, 1);

  cr->set_source (solid_pattern);
  {
    auto retrieved_solid =
      Cairo::RefPtr<Cairo::SolidPattern>::cast_dynamic(cr->get_source ());
    BOOST_REQUIRE (retrieved_solid);
    double r, g, b, a;
    retrieved_solid->get_rgba (r, g, b, a);
    BOOST_CHECK_EQUAL (1.0, r);
    BOOST_CHECK_EQUAL (0.5, g);
    BOOST_CHECK_EQUAL (0.25, b);

    // now try for const objects..
    auto cr2 = cr;
    auto retrieved_solid2 =
      Cairo::RefPtr<const Cairo::SolidPattern>::cast_dynamic(cr2->get_source ());
    BOOST_REQUIRE (retrieved_solid2);

    // Check that get_source_for_surface() returns an empty RefPtr,
    // when the source is not a surface.
    auto surface_pattern = cr->get_source_for_surface();
    BOOST_CHECK(!surface_pattern);
    auto surface_pattern2 = cr2->get_source_for_surface();
    BOOST_CHECK(!surface_pattern2);
  }

  cr->set_source (gradient_pattern);
  {
    auto retrieved_linear =
      Cairo::RefPtr<Cairo::LinearGradient>::cast_dynamic(cr->get_source ());
    BOOST_REQUIRE (retrieved_linear);
    double x0, x1, y0, y1;
    retrieved_linear->get_linear_points (x0, y0, x1, y1);
    BOOST_CHECK_EQUAL (0.0, x0);
    BOOST_CHECK_EQUAL (0.0, y0);
    BOOST_CHECK_EQUAL (1.0, x1);
    BOOST_CHECK_EQUAL (1.0, y1);
  }

  cr->set_source_rgb (1.0, 0.5, 0.25);
  {
    auto solid =
      Cairo::RefPtr<Cairo::SolidPattern>::cast_dynamic(cr->get_source ());
    BOOST_REQUIRE (solid);
    double rx, gx, bx, ax;
    solid->get_rgba (rx, gx, bx, ax);
    BOOST_CHECK_EQUAL (1.0, rx);
    BOOST_CHECK_EQUAL (0.5, gx);
    BOOST_CHECK_EQUAL (0.25, bx);
  }

  cr->set_source_rgba (0.1, 0.3, 0.5, 0.7);
  {
    auto solid =
      Cairo::RefPtr<Cairo::SolidPattern>::cast_dynamic(cr->get_source ());
    BOOST_REQUIRE (solid);
    double rx, gx, bx, ax;
    solid->get_rgba (rx, gx, bx, ax);
    BOOST_CHECK_EQUAL (0.1, rx);
    BOOST_CHECK_EQUAL (0.3, gx);
    BOOST_CHECK_EQUAL (0.5, bx);
    BOOST_CHECK_EQUAL (0.7, ax);
  }

  cr->set_source (surface, 0.0, 0.0);
  {
    auto surface_pattern = cr->get_source_for_surface();
    BOOST_REQUIRE (surface_pattern);
    surface_pattern->set_filter(Cairo::FILTER_NEAREST);
    BOOST_CHECK_EQUAL (Cairo::FILTER_NEAREST, surface_pattern->get_filter());
  }
}

BOOST_AUTO_TEST_CASE(test_tolerance)
{
  CREATE_CONTEXT(cr);
  cr->set_tolerance (3.0);
  BOOST_CHECK_EQUAL (3.0, cr->get_tolerance ());
}

BOOST_AUTO_TEST_CASE(test_antialias)
{
  CREATE_CONTEXT(cr);
  cr->set_antialias (Cairo::ANTIALIAS_GRAY);
  BOOST_CHECK_EQUAL (Cairo::ANTIALIAS_GRAY, cr->get_antialias ());

  cr->set_antialias (Cairo::ANTIALIAS_SUBPIXEL);
  BOOST_CHECK_EQUAL (Cairo::ANTIALIAS_SUBPIXEL, cr->get_antialias ());
}

BOOST_AUTO_TEST_CASE(test_fill_rule)
{
  CREATE_CONTEXT(cr);
  cr->set_fill_rule (Cairo::FILL_RULE_EVEN_ODD);
  BOOST_CHECK_EQUAL (Cairo::FILL_RULE_EVEN_ODD, cr->get_fill_rule ());
  cr->set_fill_rule (Cairo::FILL_RULE_WINDING);
  BOOST_CHECK_EQUAL (Cairo::FILL_RULE_WINDING, cr->get_fill_rule ());
}

BOOST_AUTO_TEST_CASE(test_line_width)
{
  CREATE_CONTEXT(cr);
  cr->set_line_width (1.0);
  BOOST_CHECK_EQUAL (1.0, cr->get_line_width ());
  cr->set_line_width (4.0);
  BOOST_CHECK_EQUAL (4.0, cr->get_line_width ());
}

BOOST_AUTO_TEST_CASE(test_line_cap)
{
  CREATE_CONTEXT(cr);
  cr->set_line_cap (Cairo::LINE_CAP_BUTT);
  BOOST_CHECK_EQUAL (Cairo::LINE_CAP_BUTT, cr->get_line_cap ());
  cr->set_line_cap (Cairo::LINE_CAP_ROUND);
  BOOST_CHECK_EQUAL (Cairo::LINE_CAP_ROUND, cr->get_line_cap ());
}

BOOST_AUTO_TEST_CASE(test_line_join )
{
  CREATE_CONTEXT(cr);
  cr->set_line_join (Cairo::LINE_JOIN_BEVEL);
  BOOST_CHECK_EQUAL (Cairo::LINE_JOIN_BEVEL, cr->get_line_join ());
  cr->set_line_join (Cairo::LINE_JOIN_MITER);
  BOOST_CHECK_EQUAL (Cairo::LINE_JOIN_MITER, cr->get_line_join ());
}

BOOST_AUTO_TEST_CASE(test_miter_limit)
{
  CREATE_CONTEXT (cr);
  cr->set_miter_limit (1.3);
  BOOST_CHECK_EQUAL (1.3, cr->get_miter_limit ());
  cr->set_miter_limit (4.12);
  BOOST_CHECK_EQUAL (4.12, cr->get_miter_limit ());
}

BOOST_AUTO_TEST_CASE(test_matrix)
{
  // just excercise the functionality
  CREATE_CONTEXT (cr);
  Cairo::Matrix matrix;
  cairo_matrix_init (&matrix, 1.0, 0.1, 0.1, 1.0, 1.5, 1.5);
  cr->transform(matrix);
  cairo_matrix_init (&matrix, 1.0, -0.1, -0.1, 1.0, 1.5, 1.5);
  cr->set_matrix(matrix);
  cr->set_identity_matrix ();
  cr->get_matrix (matrix);
  auto m2 = cr->get_matrix ();
  (void)m2; // Silence a warning (unused-but-set-variable)
}

BOOST_AUTO_TEST_CASE(test_user_device)
{
  // scale / transform a context, and then verify that user-to-device and
  // device-to-user things work.
  CREATE_CONTEXT (cr);
  cr->scale (2.3, 2.3);
  double x = 1.8, y = 1.8;
  cr->user_to_device (x, y);
  // x = (0.0 + x) * 2.3 => 1.8 * 2.3 = 5.29
  BOOST_CHECK_EQUAL (4.14, x);
  BOOST_CHECK_EQUAL (4.14, y);
  cr->device_to_user (x, y);
  BOOST_CHECK_EQUAL (1.8, x);
  BOOST_CHECK_EQUAL (1.8, y);
  cr->translate (0.5, 0.5);
  cr->user_to_device (x, y);
  // x = (0.5 + x) * 2.3 => 2.3 * 2.3 = 5.29
  BOOST_CHECK_CLOSE (5.29, x, FLT_EPSILON);
  BOOST_CHECK_CLOSE (5.29, y, FLT_EPSILON);
}

BOOST_AUTO_TEST_CASE(test_draw)
{
  CREATE_CONTEXT (cr);
  // just call a bunch of drawing functions to excercise them a bit.  There's no
  // rhyme or reason to this, don't expect it to draw anything interesting.
  cr->begin_new_path ();
  cr->move_to (1.0, 1.0);
  cr->line_to (2.0, 2.0);
  cr->curve_to (0.5, 0.5, 0.5, 0.5, 1.0, 1.0);
  cr->arc (1.5, 0.5, 0.5, 0, 2 * M_PI);
  cr->stroke ();
  cr->arc_negative (1.5, 0.5, 0.5, 0, 2 * M_PI);
  cr->rel_move_to (0.1, 0.1);
  cr->rel_line_to (0.5, -0.5);
  cr->rel_curve_to (0.5, 0.5, 0.5, 0.5, 1.0, 1.0);
  cr->rectangle (0.0, 0.0, 1.0, 1.0);
  cr->close_path ();
  cr->paint ();
}

BOOST_AUTO_TEST_CASE(test_clip)
{
  CREATE_CONTEXT (cr);
  cr->rectangle (0.0, 0.0, 1.0, 1.0);
  cr->clip ();
  double x1, y1, x2, y2;
  cr->get_clip_extents (x1, y1, x2, y2);
  BOOST_CHECK (x1 == 0.0);
  BOOST_CHECK (y1 == 0.0);
  BOOST_CHECK (x2 == 1.0);
  BOOST_CHECK (y2 == 1.0);
}

BOOST_AUTO_TEST_CASE(test_current_point)
{
  CREATE_CONTEXT (cr);
  cr->move_to (2.0, 3.0);
  double x, y;
  cr->get_current_point (x, y);
  BOOST_CHECK (x == 2.0);
  BOOST_CHECK (y == 3.0);
}

BOOST_AUTO_TEST_CASE(test_target)
{
  auto surf = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 10, 10); \
  auto cr = Cairo::Context::create(surf);

  auto target_surface =
    Cairo::RefPtr<Cairo::ImageSurface>::cast_dynamic(cr->get_target ());
  auto bad_surface =
    Cairo::RefPtr<Cairo::PdfSurface>::cast_dynamic(cr->get_target ());
  BOOST_CHECK (target_surface);
  BOOST_CHECK (!bad_surface);

  // now check for const objects...
  auto cr2 = Cairo::Context::create(surf);

  auto target_surface2 =
    Cairo::RefPtr<const Cairo::ImageSurface>::cast_dynamic(cr2->get_target ());
  auto bad_surface2 =
    Cairo::RefPtr<const Cairo::PdfSurface>::cast_dynamic(cr2->get_target ());
  BOOST_CHECK (target_surface2);
  BOOST_CHECK (!bad_surface2);
}

BOOST_AUTO_TEST_CASE(test_scaled_font)
{
  CREATE_CONTEXT (cr);
  auto face = Cairo::ToyFontFace::create("sans",
                                                                      Cairo::FONT_SLANT_NORMAL,
                                                                      Cairo::FONT_WEIGHT_NORMAL);
  Cairo::Matrix identity;
  cairo_matrix_init_identity(&identity);
  auto font = Cairo::ScaledFont::create(face,
                                                                    identity,
                                                                    identity,
                                                                    Cairo::FontOptions());
  BOOST_CHECK_NO_THROW(cr->set_scaled_font(font));
  // There's no operator== for ScaledFont, so I'm not sure if there's a way to
  // compare whether the font we get form get() is the same as the one we set
  // The following test fails
  //BOOST_CHECK_EQUAL(font, cr->get_scaled_font());

  //just put in a pseudo-test for now to excercise the API
  BOOST_CHECK(cr->get_scaled_font());
}

BOOST_AUTO_TEST_CASE(test_font_options)
{
  CREATE_CONTEXT (cr);
  Cairo::FontOptions options;
  options.set_antialias(Cairo::ANTIALIAS_GRAY);
  cr->set_font_options(options);
  Cairo::FontOptions other;
  cr->get_font_options(other);
  BOOST_CHECK(options == other);
}

BOOST_AUTO_TEST_SUITE_END()
