#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
using namespace boost::unit_test;
#include <cairomm/surface.h>
using namespace Cairo;

static unsigned int test_slot_called = 0;
ErrorStatus test_slot(const unsigned char* /*data*/, unsigned int /*len*/)
{
  test_slot_called++;
  return CAIRO_STATUS_SUCCESS;
}

BOOST_AUTO_TEST_SUITE( Cairo_Surface )

BOOST_AUTO_TEST_CASE(test_write_to_png_stream)
{
  auto surface = ImageSurface::create(FORMAT_ARGB32, 1, 1);
  surface->write_to_png_stream(sigc::ptr_fun(test_slot));
  BOOST_CHECK(test_slot_called > 0);
}

BOOST_AUTO_TEST_CASE(test_pdf_constructor_slot)
{
  test_slot_called = 0;
  auto pdf = PdfSurface::create_for_stream(sigc::ptr_fun(&test_slot), 1, 1);
  pdf->show_page();
  pdf->finish();
  BOOST_CHECK(test_slot_called > 0);
}

BOOST_AUTO_TEST_CASE(test_ps_constructor_slot)
{
  test_slot_called = 0;
  auto ps = PsSurface::create_for_stream(sigc::ptr_fun(&test_slot), 1, 1);
  ps->show_page();
  ps->finish();
  BOOST_CHECK(test_slot_called > 0);
}

BOOST_AUTO_TEST_CASE(test_svg_constructor_slot)
{
  test_slot_called = 0;
  auto svg = SvgSurface::create_for_stream(sigc::ptr_fun(&test_slot), 1, 1);
  svg->show_page();
  svg->finish();
  BOOST_CHECK(test_slot_called > 0);
}

static std::ifstream png_file;
unsigned int test_read_func_called = 0;
static ErrorStatus test_read_func(unsigned char* data, unsigned int len)
{
  ++test_read_func_called;
  if (png_file.read(reinterpret_cast<char*>(data), len))
    return CAIRO_STATUS_SUCCESS;
  return CAIRO_STATUS_READ_ERROR;
}

unsigned int c_test_read_func_called = 0;
static cairo_status_t c_test_read_func(void* /*closure*/, unsigned char* data, unsigned int len)
{
  ++c_test_read_func_called;
  if (png_file.read(reinterpret_cast<char*>(data), len))
    return CAIRO_STATUS_SUCCESS;
  return CAIRO_STATUS_READ_ERROR;
}

BOOST_AUTO_TEST_CASE(test_create_from_png)
{
  RefPtr<ImageSurface> surface;
  // try the sigc::slot version
  png_file.open(PNG_STREAM_FILE);
  surface = ImageSurface::create_from_png_stream(sigc::ptr_fun(&test_read_func));
  png_file.close();
  BOOST_CHECK(test_read_func_called > 0);

  // now try the raw C function (deprecated) version
  png_file.open(PNG_STREAM_FILE);
  surface = ImageSurface::create_from_png(&c_test_read_func, nullptr);
  png_file.close();
  BOOST_CHECK(c_test_read_func_called > 0);
}

BOOST_AUTO_TEST_CASE(test_ps_eps)
{
  auto ps = PsSurface::create("test.ps", 1, 1);
  // check the initial value
  bool result = ps->get_eps();
  // set it to the opposite value
  ps->set_eps(!result);
  // verify
  BOOST_CHECK_EQUAL(ps->get_eps(), !result);
}

BOOST_AUTO_TEST_CASE(test_content)
{
  auto surface = ImageSurface::create(FORMAT_ARGB32, 1, 1);
  BOOST_CHECK_EQUAL(surface->get_content(), CONTENT_COLOR_ALPHA);
  auto similar = Surface::create(surface, CONTENT_ALPHA, 1, 1);
  BOOST_REQUIRE(similar);
  BOOST_CHECK_EQUAL(similar->get_content(), CONTENT_ALPHA);
}

BOOST_AUTO_TEST_CASE(test_fallback_resolution)
{
  auto surface = ImageSurface::create(FORMAT_ARGB32, 1, 1);
  double x, y;
  surface->get_fallback_resolution(x, y);
  const double new_x = 94, new_y = 123;
  surface->set_fallback_resolution(new_x, new_y);
  surface->get_fallback_resolution(x, y);
  BOOST_CHECK_EQUAL(x, new_x);
  BOOST_CHECK_EQUAL(y, new_y);
}

BOOST_AUTO_TEST_CASE(test_show_text_glyphs)
{
  // image surface doesn't support show_text_glyphs
  Cairo::RefPtr<Cairo::Surface> surf = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 10, 10);
  BOOST_CHECK(!surf->has_show_text_glyphs());
  // but pdf surface should
  surf = Cairo::PdfSurface::create("test.pdf", 10.0, 10.0);
  BOOST_CHECK(surf->has_show_text_glyphs());
}

BOOST_AUTO_TEST_SUITE_END()
