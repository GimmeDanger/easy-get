# easy-get

C++ http client for web files downloading

## Building in Linux 

Get sources & install:
<pre>
git clone https://github.com/GimmeDanger/easy-get.git
bash build.sh
</pre>

Building script runs unit tests at the end:
<pre>
url_construction_test OK
parse_input_options_test OK
parse_input_urls_test OK
http_queries_simple_html_test OK
http_queries_picture_test OK
http_queries_video_test OK
http_queries_pdf_test OK
</pre>

## Usage
<pre>
usage: easy-get [OPTIONS] URL ...
-n NUM: available number of thread
-h    : print help message and exit
Expected URL format: http://HOSTNAME/[PATH]
</pre>

## Examples
Downloading of a random video sample and picture:
<pre>
./easy_get -n 2 http://mirrors.standaloneinstaller.com/video-sample/star_trails.mp4 http://st.kp.yandex.net/images/kadr/sm_2562186.jpg
</pre>
