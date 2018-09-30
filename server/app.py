#!/usr/bin/python3
#
import os
import pathlib
from http.server import BaseHTTPRequestHandler,HTTPServer

import json

client_path = 'client/'

def postHandler(post_bytes):
    # Convert to string.
    post_string = post_bytes.decode("utf-8")
    try:
        # Convert string to json.
        post_json = json.JSONDecoder().decode(post_string)
        # Verify function.
        try:
            fcn = post_json['fcn']
        except KeyError:
            result_string = '{"error":"No function"}'
            return result_string
        if fcn != 'calculate':
            result_string = '{"error":"Unknown function"}'
            return result_string
        # Verify arguments.
        try:
            arg = post_json['arg']
        except KeyError:
            result_string = '{"error":"No arguments"}'
            return result_string
        if len(arg) != 3:
            result_string = '{"error":"Wrong number of arguments"}'
            return result_string
        # Calculate the sum.
        arg0 = arg[0]
        arg1 = arg[1]
        arg2 = arg[2]
        sum = arg0 + arg1 + arg2
        # Create the result.
        result = {'result':sum}
        result_string = json.JSONEncoder().encode(result)
        # And return.
        return result_string

    except json.JSONDecodeError:
        post_string = 'Decode Error'


class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):

        result_code = 200
        content_type = 'text/html'

        # Get the requested file with path.
        file_path = self.path
        # If the path is '/' or 0-length.
        if file_path == '/' or len(file_path) == 0:
            # Going with index.
            file_path = 'index.html'
        # Get the file name.
        file_name = os.path.basename(file_path)
        # Get the file extension.
        ext = pathlib.Path(file_name).suffix[1:]
        # One of our known types?
        if ext == 'html' or ext == 'js' or ext == 'css' or ext == 'ts' or ext == 'map':
            # Create relative path.
            rel_name = client_path + file_name
            # If the file exists.
            if os.path.isfile(rel_name):
                # Open the file.
                fd = open(rel_name , 'r')
                # Read it.
                file_data = fd.read()
                # Close it.
                fd.close()
                # If css was requested we must modify the content for IE.
                if ext == 'css':
                    content_type = 'text/css'
            # Else file does not exist.
            else:
                result_code = 404
        # Else we don't handle it.
        else:
            result_code = 404
        # If file not present.
        if result_code == 404:
            file_data = 'File not found'
        # Send the headers.
        self.send_response(result_code)
        self.send_header('Content-type' , content_type)
        self.end_headers()
        # Send the response.
        self.wfile.write(bytes(file_data , 'UTF-8'))
        return

    def do_POST(self):
        # Originally 'content-length', but Postman uses 'Content-Length'???
        try:
            content_str = self.headers['content-length']
        except KeyError:
            try:
                content_str = self.headers['Content-Length']
            except KeyError:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(bytes('Unknown content length'))
                return
        content_len = int(content_str)
        # Data returned as byte[].
        post_bytes = self.rfile.read(content_len)
        # Send to post handler.
        result_string = postHandler(post_bytes)
        # Send the headers.
        self.send_response(200)
        self.end_headers()
        # Send the result.
        self.wfile.write(bytes(result_string , 'UTF-8'))
        return

if __name__ == '__main__':

    # If we were not started from the top level.
    if not os.path.isdir('client'):
        # Need to redefine the client folder.
        client_path = '../client/'
    server = HTTPServer(('localhost', 8080) , RequestHandler)
    print('Starting server at http://localhost:8080')
    server.serve_forever()
