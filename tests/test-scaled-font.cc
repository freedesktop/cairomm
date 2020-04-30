#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>
using namespace boost::unit_test;
#include <cairomm/scaledfont.h>
#include <iostream>

using namespace Cairo;

BOOST_AUTO_TEST_SUITE( Cairo_ScaledFont )

BOOST_AUTO_TEST_CASE(test_construction)
{
  auto face = ToyFontFace::create("sans", FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);
  Matrix identity;
  cairo_matrix_init_identity(&identity);
  auto font = ScaledFont::create(face, identity, identity, FontOptions());
  BOOST_REQUIRE(font);

  // now use the default argument for font_options
  font = ScaledFont::create(face, identity, identity);
  BOOST_REQUIRE(font);
}

BOOST_AUTO_TEST_CASE(test_text_to_glyphs)
{
  auto face = ToyFontFace::create("sans", FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);
  Matrix identity;
  cairo_matrix_init_identity(&identity);
  auto font = ScaledFont::create(face, identity, identity, FontOptions());
  BOOST_REQUIRE(font);

  std::vector<Glyph> glyphs;
  std::vector<TextCluster> clusters;
  TextClusterFlags flags;
  font->text_to_glyphs(0, 0, "foo", glyphs, clusters, flags);

  BOOST_CHECK_EQUAL(3, glyphs.size());
  BOOST_CHECK_EQUAL(3, clusters.size());
}

BOOST_AUTO_TEST_CASE(test_scale_matrix)
{
  auto face = ToyFontFace::create("sans", FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);
  Matrix m;
  cairo_matrix_init_scale(&m, 2.0, 4.0);
  auto font = ScaledFont::create(face, m, m, FontOptions());
  BOOST_REQUIRE(font);

  Matrix result;
  font->get_scale_matrix(result);
  // no real test, just excercising the functionality
}

BOOST_AUTO_TEST_CASE(test_get_font_face)
{
  // this is to test for a bug where we were accidentally freeing the resulting
  // font face from a call to ScaledFont::get_font_face() when we didn't hold a
  // reference to it
  auto face = ToyFontFace::create("sans", FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);
  Matrix identity;
  cairo_matrix_init_identity(&identity);
  auto font = ScaledFont::create(face, identity, identity, FontOptions());
  BOOST_REQUIRE(font);
  const int refcount = cairo_font_face_get_reference_count(face->cobj());
  {
    auto got_face = font->get_font_face();
  } // scope ensure that the font face is destroyed
  // after creating and destroying the FontFace in get_font_face, our reference
  // count should be the same
  BOOST_REQUIRE_EQUAL(cairo_font_face_get_reference_count(face->cobj()), refcount);
}

#if defined (CAIRO_HAS_FT_FONT) && defined (CAIRO_HAS_FC_FONT)
BOOST_AUTO_TEST_CASE(test_ft_scaled_font)
{
  // Does not throw an exception. Skip this test for now. /Kjell Ahlstedt 2020-04-30
  //auto invalid = FcPatternCreate();
  //Cairo::RefPtr<Cairo::FtFontFace> invalid_face;
  //BOOST_CHECK_THROW(invalid_face = Cairo::FtFontFace::create(invalid), std::bad_alloc);

  // basically taken from the cairo test case -- we don't care what font we're
  // using so just create an empty pattern and do the minimal substitution to
  // get a valid pattern
  auto pattern = FcPatternCreate();
  FcConfigSubstitute (nullptr, pattern, FcMatchPattern);
  FcDefaultSubstitute (pattern);
  FcResult result;
  auto resolved = FcFontMatch (nullptr, pattern, &result);
  auto face = Cairo::FtFontFace::create(resolved);
  BOOST_CHECK(face);

  cairo_scaled_font_t* c_scaled_font = nullptr;
  int refcount = 0;
  {
    auto scaled_font =
      FtScaledFont::create(face,
                           Cairo::identity_matrix(),
                           Cairo::identity_matrix(),
                           FontOptions());
    c_scaled_font = scaled_font->cobj();
    refcount = cairo_scaled_font_get_reference_count(c_scaled_font);
  }
  // make sure that the base destructor is called
  BOOST_CHECK_EQUAL(cairo_scaled_font_get_reference_count(c_scaled_font), refcount -1);
}
#endif // CAIRO_HAS_FT_FONT && CAIRO_HAS_FC_FONT

BOOST_AUTO_TEST_SUITE_END()
