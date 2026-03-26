/* biblelookupserver.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * Server that receives info from the client, performs the lookup,
 * and then sends that information back to the client
 * 
 * STUDENT NAME: Solomon Robinson
 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "fifo.h"
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"

#define logging
#define LOG_FILENAME "/tmp/solrobinsion-biblelookupserver.log"
#include "logfile.h"

using namespace std;


/* Server main line,create name MAP, wait for and serve requests */
int main()
{
   #ifdef logging logFile.open(LogFile, ios::out); #endif


   string line;  // A line from the text file
   string reply; //reply to send back to client
   LookupResult status; //for lookup

   /* Create the communication fifos */
   Fifo recfifo("bibleRequest");
   Fifo sendfifo("bibleReply");

   recfifo.openread();
   sendfifo.openwrite();

   // "infinite loop" for server: repeat each time a request is received
   while (true)
   {
      /* Get a message from a client */
      string request = recfifo.recv();
      cout << "(SERVER) Request received: " << request << endl;
      log(request);

      if (request == "" || request == "$end"){
         continue; //finish loop iteration with invalid input or end
      }

      Bible webBible("/home/class/csc3004/Bibles/web-complete"); // Bible for lookup

      // Parse thru
      stringstream ss(request);
      string book, chapter, verseStr, numOfVerses;

      getline(ss,book, ':');
      getline(ss,chapter, ':');
      getline(ss,verseStr, ':');
      getline(ss,numOfVerses, ':');
      
      //convert to int for lookup!
      int bookI, chapterI, verseI, numOfVersesI;

      bookI = stoi(book);
      chapterI = stoi(chapter);
      verseI = stoi(verseStr);
      numOfVersesI = stoi(numOfVerses);


      Ref ref(bookI,chapterI,verseI); //standard stuff for lookup
      Verse verse = webBible.lookup(ref,status);
      string reply = ref.getBookName() + " " + chapter + ":" + verseStr + " (" + numOfVerses + " verses) " + verse.getVerse(); //populate our reply!


      if (status == SUCCESS){
         sendfifo.send(reply); //send out reply
         log("Server requesting " + book + ":" + chapter + ":" + verseStr + ":" + numOfVerses);
         cout << "(SERVER) Requesting " + book + ":" + chapter + ":" + verseStr + ":" + numOfVerses << endl;

         for (int i = 1; i < numOfVersesI; i++){ //multiple verse implementation
            verse = webBible.nextVerse(status);

            reply = verse.getVerse();
            sendfifo.send(reply); //semd each verse individually over pipe
         }
      } 
      else {
         sendfifo.send(webBible.error(status));
         log("Result status: " + webBible.error(status));
         cout << "Result status: " + webBible.error(status) << endl;
      }
      sendfifo.send("$end"); //end
      cout << endl << "(SERVER) SENDING $end" << endl;
      log("(SERVER) SENDING $end");
   }
  recfifo.fifoclose(); 
  sendfifo.fifoclose();
}
#endif
