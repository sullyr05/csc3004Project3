/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Solomon Robinson 
 */
#include "fifo.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

#define logging
#define LOG_FILENAME "/tmp/solrobinson-testreader.log"
#include "logfile.h"


/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int main()
{
   #ifdef logging logFile.open(LogFile, ios::out); #endif

   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";
   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");
    

   string bookNum = book->getValue();
   string verseNum = verse->getValue(); //assign values from form iterators to variables for sending on the pipe
   string chapterNum = chapter->getValue();
   string numOfVerses = nv->getValue();

   Fifo sendFifo("bibleRequest");
   Fifo recFifo("bibleReply");

   sendFifo.openwrite();

   string req = bookNum + ":" + chapterNum + ":" + verseNum + ":" + numOfVerses; //send request
   log("(CLIENT) " << req << endl);
   sendFifo.send(req);

   recFifo.openread(); // receive reply
   string reply = "";
   for (reply = recFifo.recv(); reply != "$end"; reply = recFifo.recv()) {
   cout << "<p>" << reply << "</p>";
   log("(CLIENT) " << reply << endl);
   }

   recFifo.fifoclose();
   sendFifo.fifoclose();
   log("(CLIENT) Reply fifo closed" << endl);

   return 0;
}
#endif
