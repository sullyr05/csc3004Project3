/* testreader.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * Test program to test the new Indexed Bible class
 * 
 *
 * STUDENT NAME: Solomon Robinson
 */

#include <iostream>
#include <string>
#include "fifo.h"
#define logging
#define LOG_FILENAME "/tmp/solrobinson-testreader.log"
#include "logfile.h"

using namespace std;

int main (int argc, char **argv)
{
   #ifdef logging logFile.open(LogFile, ios::out); #endif


   string bookNum, chapterNum, verseNum, numOfVerses;
   numOfVerses = "1"; // default one verse
   
   if (argc < 4 || argc > 5){
      cerr << "Wrong Usage! Proper Usage : " << argv[0] << " 1 1 1 (1)" << endl;
      return 1;
   }

   bookNum = argv[1];
   chapterNum = argv[2]; // cli implementation
   verseNum = argv[3];

   if (argc > 4){
      numOfVerses = argv[4];
   }


   Fifo sendFifo("bibleRequest");
   Fifo recFifo("bibleReply");

   sendFifo.openwrite();

   string req = bookNum + ":" + chapterNum + ":" + verseNum + ":" + numOfVerses; //send request
   log("(CLIENT) " << req << endl);
   cout << "(CLIENT) " << req << endl;
   sendFifo.send(req);

   recFifo.openread(); // receive reply
   string reply = "";
   while (reply != "$end") {
      reply = recFifo.recv();
      cout << reply << endl; //display verse
      log("(CLIENT) " << reply << endl); //log verse
   }

   recFifo.fifoclose();
   sendFifo.fifoclose();
   log("(CLIENT) Reply fifo closed" << endl);
   return 0;
}
#endif