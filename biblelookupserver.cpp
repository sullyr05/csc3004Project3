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
   LookupResult status = OTHER; //for lookup

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
      cout << "(SERVER) STATUS VARIABLE = " << status;

      if (ref.isRefValid(ref) && status == SUCCESS){
         reply = verse.getRef().toString() +
         " (" + numOfVerses + " verses) " + verse.getVerse(); //populate our reply!

         sendfifo.send(reply); //send out reply
         log("Server requesting " + book + ":" + chapter + ":" + verseStr + ":" + numOfVerses);
         cout << "(SERVER) Requesting " + book + ":" + chapter + ":" + verseStr + ":" + numOfVerses << endl;

         for (int i = 1; i < numOfVersesI; i++){ //multiple verse implementation
            Verse tempV = verse;
            verse = webBible.nextVerse(status);
            if (verse.getRef().getChapter() != tempV.getRef().getChapter()){ //newline between chapters
               reply = verse.getRef().toString() + " " + verse.getVerse() + "\n\n";
               sendfifo.send(reply);
               }
            else {
            reply = verse.getRef().toString() + " " + verse.getVerse();
            sendfifo.send(reply); //send each verse individually over pipe
            }
         }
      } 
      else if (ref.getChapter() <= 0 || ref.getVerse() <= 0){
         cout << "You must enter a positive integer for chapter and verse numbers. ";
         log ("Negative integer attempted for chapter/verse");
         sendfifo.send("Please enter a positive integer for chapter and verse numbers!");
      }
      else {
         sendfifo.send(webBible.error(status) + "\n There is no verse " + 
         verseStr + " in chapter " + chapter + " of the book of that book!\n");
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
