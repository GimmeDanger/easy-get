#include "url.h"
#include "http_client.h"
#include "test_runner.h"

#include <string>
#include <sstream>

void http_querries_test(std::vector<url> urls) {
  for (const auto &url : urls) {
    http_client client(url.get_host());
    auto verdict = client.get(url.get_path());
    ASSERT_EQUAL(http_client::to_string (verdict), http_client::to_string (http_client::get_code_t::OK));
  }
}

void http_queries_simple_html_test() {
  std::vector<url> urls = {
      // urls without file path, they should be saved as .html
      url("http://algolist.ru"),
      url("http://government.ru/news/40062/")
  };
  http_querries_test (urls);
}

void http_queries_picture_test() {
  std::vector<url> urls = {
      // some random pictures
      url("http://st.kp.yandex.net/images/kadr/sm_2562186.jpg"),
      url("http://nas-national-prod.s3.amazonaws.com/styles/hero_cover_bird_page/s3/h_apa_2016-a1_1902_7_red-shouldered-hawk_sandra_rothenberg_kk_adult-florida.jpg"),
      url("http://ichef.bbci.co.uk/live-experience/cps/624/cpsprodpb/vivo/live/images/2020/7/19/037e70cf-073d-44a7-adaa-dd5e1d31214d.jpg"),
  };
  http_querries_test (urls);
}

void http_queries_video_test() {
  std::vector<url> urls = {
      // some random video samples
      url("http://mirrors.standaloneinstaller.com/video-sample/metaxas-keller-Bell.3gp"),
      url("http://mirrors.standaloneinstaller.com/video-sample/star_trails.mp4")
  };
  http_querries_test (urls);
}

void http_queries_pdf_test() {
  std::vector<url> urls = {
      // pdf from russian gov website
      url("http://static.government.ru/media/files/nROGq1gQxivMU8zyXfEZdKqFM6XXVbum.pdf")
  };
  http_querries_test (urls);
}

int main()
{
  TestRunner tr;
  RUN_TEST (tr, http_queries_simple_html_test);
  RUN_TEST (tr, http_queries_picture_test);
  RUN_TEST (tr, http_queries_video_test);
  RUN_TEST (tr, http_queries_pdf_test);
  return 0;
}