/*
 * main.cpp
 *
 *  Created on: Sep 29, 2018
 *      Author: rcw
 */

#include 	<mongoose.h>
#include	<jansson.hpp>

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static int  dbug_count;

static json_t* processPost(mg_str* body)
{
	json_value_t	v;
	json_value_t	arg1 , arg2 , arg3;
	int32_t			retval;
	json_t*			arr = NULL;
	json_t*			retObj = NULL;
	json_t*			req = jsonNew((char*)body->p , body->len , NULL);

	if (req != NULL)
	{
		// Get the function.
		v = req->value("fcn");
		if (v.isString())
		{
			/*
			 * TODO: Route here to specific function.
			 *
			 * Functions should probably be sent as a number, so
			 * that we can use a switch() to route.
			 *
			 * Additionally, we should pass an error string that
			 * can be set if problems encountered.
			 */
			if (strcmp(v.str.c_str() , "calculate") == 0)
			{
				arr = req->child("arg");
				if ((arr != NULL) && (arr->type == JSON_ARRAY))
				{
					if (arr->arraySize() == 3)
					{
						arg1 = arr->value(0);
						arg2 = arr->value(1);
						arg3 = arr->value(2);

						retval  = arg1.num.i;
						retval += arg2.num.i;
						retval += arg3.num.i;

						retObj = jsonNew();
						retObj->addChild("result" , retval);
					}
				}
			}
		}
		// Release memory.
		jsonDel(req);
		if (arr != NULL)
			jsonDel(arr);
	}
	// If error.
	if (retObj == NULL)
	{
		retObj = jsonNew();
		retObj->addChild("error" , "parsing or argument error");
	}
	// And return.
	return retObj;
}

static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
	http_message*	msg = (struct http_message*)p;
    json_t*			retObj;
    string			retStr;

    if (ev == MG_EV_HTTP_REQUEST)
    {
        // GET
        if (mg_vcmp(&msg->method , "GET") == 0)
        {
            mg_serve_http(nc, msg, s_http_server_opts);
        }
        // POST
        else if (mg_vcmp(&msg->method , "POST") == 0)
        {
        	// Process the request.
            retObj = processPost(&msg->body);
            // Create pretty JSON string.
            retObj->toString(&retStr , 4);
            // Release memory.
            jsonDel(retObj);
            // Send response to client.
            mg_send_head(nc , 200 , retStr.size() , "Content-Type: application/json");
            mg_send(nc , retStr.data() , retStr.size());
        }
        // Not supported
        else
        {

        }
    }
    else if (ev == MG_EV_SEND)
    {
        ++dbug_count;
    }
}

int main(void)
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr, NULL);
    printf("Starting web server on port %s\n", s_http_port);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL)
    {
        printf("Failed to create listener\n");
        return 1;
    }

    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = "./client";
    s_http_server_opts.enable_directory_listing = "yes";

    for (; ;)
    {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}



