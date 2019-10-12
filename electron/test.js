var http = require('http'); 

function send_command()
{
    //window.alert("hello test");

    if($("#command").val() == "") 
    {   $("#status").html("error : command is empty")
        return;
    }
    const options = {
        hostname: 'localhost',
        port: 8080,
        path: '/do?action=ceci&param=cela',
        method: 'GET'
      }
      options.path = "/do" + "?action=" + $("#command").val() + "&param=" + $("#param").val()
      console.log(options.path)
      const req = http.request(options, res => {
        console.log(`statusCode: ${res.statusCode}`)
        $("#status").html(`Status code  : ${res.statusCode} ${res.statusMessage} `)
        res.on('data', message => {
           console.log("data :" + message)
           $("#response").html(" " + message)
        })
      })
      
      req.on('error', error => {
        console.error(error)
      })
      
      req.end()
}