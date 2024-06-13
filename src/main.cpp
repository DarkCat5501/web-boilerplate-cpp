#include <vendor/httplib.h>
#include <vendor/html.h>
#include <sstream>

std::atomic_int acc_counter = 0;

int main(int argc, const char **argv) {
  if (argc > 1 && std::string("--help") == argv[1]) {
    printf("usage: main [PORT]\n"); return 1;
  }

  //Start routings
  http::Server app; 

  //mount the app at the the www folder
  app.set_mount_point("/","./public");
  //overload the css handler
  app.set_pre_routing_handler([](const http::Request& req,http::Response& res) {
  if (req.path == "/css/styles.css") {
    res.set_content("body{background:black;color:white;}", "text/css");
    return http::Server::HandlerResponse::Handled;
  }
  return http::Server::HandlerResponse::Unhandled;
});

  //enable routing for index
  app.Get("/index.html",[](const http::Request &req, http::Response &res) {
    res.set_redirect("/");
  });
 
  app.Get("/css/styles.css",[](const http::Request &req, http::Response &res) {
     res.set_content("body{background:black;color:white;}","text/css");
  });

  app.Get("/",[](const http::Request &req, http::Response &res) {
    std::stringstream page;
  
    //increment access counter
    acc_counter.fetch_add(1);

    { //Build the page
      page << 
      html::html5 << 
      html::html << 
      html::head <<
        html::link[{
          html::rel("stylesheet"),
          html::href("css/styles.css")
        }] <<
        html::script[{
          html::language("javascript"),
          html::attr("defer","defer")
        }] <<  
        "function sayHello(){" 
        "console.log('Hello world')"
        "}" <<
        html::script::end <<
      html::head::end <<
      html::body <<
        html::div[{
          html::id("app"),
          html::class_("test")
        }] <<
          "Accesses: " << acc_counter.load() <<
        html::div::end <<
      html::body::end <<
      html::end;
    }

    res.set_content(page.str(),"text/html");
  });
  
  //enable routing for errors
  app.set_error_handler([](const http::Request req, http::Response &res) {
    // req.path = "";
    res.set_redirect("/error.html");
  });

  int port = 8080;
  if (argc > 1) { port = atoi(argv[1]); }

  printf("The server started at port %d\n",port);
  app.listen("localhost", port);

  return 0;
}
