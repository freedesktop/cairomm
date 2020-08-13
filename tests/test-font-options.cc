#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <cairomm/fontoptions.h>

using namespace boost::unit_test;
using namespace Cairo;

BOOST_AUTO_TEST_SUITE( Cairo_FontOptions )

BOOST_AUTO_TEST_CASE(test_excercise)
{
  // just excercise all of the methods
  Cairo::FontOptions options;

  Cairo::FontOptions other;
  options.merge(other);

  options.hash();

  options.set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
  auto antialias = options.get_antialias();
  BOOST_CHECK_EQUAL(Cairo::ANTIALIAS_SUBPIXEL, antialias);

  options.set_subpixel_order(Cairo::SUBPIXEL_ORDER_DEFAULT);
  auto order = options.get_subpixel_order();
  BOOST_CHECK_EQUAL(Cairo::SUBPIXEL_ORDER_DEFAULT, order);

  options.set_hint_style(Cairo::HINT_STYLE_SLIGHT);
  auto hint_style = options.get_hint_style();
  BOOST_CHECK_EQUAL(Cairo::HINT_STYLE_SLIGHT, hint_style);

  options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
  auto metrics = options.get_hint_metrics();
  BOOST_CHECK_EQUAL(Cairo::HINT_METRICS_OFF, metrics);
}

BOOST_AUTO_TEST_SUITE_END()
