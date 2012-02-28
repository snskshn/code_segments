import java.net.*;
import java.io.*;

class URLTest
{
    public static void main(String args[]) throws MalformedURLException
    {
	URL url = new URL("http://www.future.co.kr:70/index.html");
	//URL url = new URL("http", "google.co.kr", 8080, "/index.html");

	System.out.println("Port: " + url.getPort());
	System.out.println("Protocol: " + url.getProtocol());
	System.out.println("HostName: " + url.getHost());
	System.out.println("File: " + url.getFile());
	System.out.println("Ref: " + url.getRef());

	System.out.println(url);

    }
};
