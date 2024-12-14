import subprocess
import shutil
import os
import mimetypes
import re
import os

filename = os.path.join(os.path.abspath(os.getcwd()), 'content')
print(filename)

def setup_web_content():
    path = './content/dist'
    target = './src/WebContent'
    
    shutil.rmtree(path, ignore_errors=True)
    subprocess.check_call('npm install', shell=True,cwd='./content')
    subprocess.check_call('npm run build', shell=True,cwd='./content')



    write_cpp_file(target, path)
    write_hpp_file(target, path)


def write_cpp_file(target, path):
    template_start = """
#include <ESPAsyncWebServer.h>
#include <WebContent.hpp>

class WebContent {
  public:
    static void setupStaticWebContent(AsyncWebServer *webserver) {
        webserver->rewrite("/", "/index.html");
"""
    templateReg = """
        webserver->on("/{path}", HTTP_GET, [](AsyncWebServerRequest *request){{
            request->send_P(200, F("{mime}"), {arr}, {len});
        }});
"""
    templateGzip = """
        webserver->on("/{path}", HTTP_GET, [](AsyncWebServerRequest *request){{
            AsyncWebServerResponse *response = request->beginResponse_P(200, F("{mime}"), {arr}, {len});
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        }});
"""
    template_end = """    }
};
"""
    gz_files = {file[:-3] for file in os.listdir(path) if file.endswith('.gz')}

    with open(f'{target}.cpp', 'w') as c:
        c.write(template_start)
        for filename in os.listdir(path):
            if filename not in gz_files:
                filesize = os.path.getsize(os.path.join(path, filename))
                if filename.endswith('.gz'):
                    content = templateGzip
                    url = filename[:-3]
                else: 
                    content = templateReg
                    url = filename

                filename_k = re.sub("[^a-zA-Z0-9]", "_", filename, count=0, flags=0)
                mime_type = mimetypes.guess_type(filename)[0]
                c.write(content.format(path=url, arr=filename_k, len=filesize, mime=mime_type))
        c.write(template_end)

def write_hpp_file(target, path):
    gz_files = {file[:-3] for file in os.listdir(path) if file.endswith('.gz')}

    with open(f'{target}.hpp', 'w') as h:
        for filename in os.listdir(path):
            if filename not in gz_files:
                filepath = os.path.join(path, filename)
                filesize = os.path.getsize(filepath)
                filename_k = re.sub("[^a-zA-Z0-9]", "_", filename, count=0, flags=0)
                h.write("#define {}_len {}\n".format(filename_k, filesize))
                h.write(f"const uint8_t {filename_k}[] PROGMEM = {{")
                with open(filepath, 'rb') as f:
                    bytes = f.read()
                    h.write(','.join([f"0x{byte:02x}" for byte in bytes]))
                h.write("};\n\n")

setup_web_content()
