/* testreader.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * Test program to test the new Indexed Bible class
 * 
 *
 * STUDENT NAME: Solomon Robinson
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char **argv)
{
   // Create Bible object to process the raw text file
   Bible webBible("/home/class/csc3004/Bibles/web-complete");

   Verse verse;
   int bookNum, chapterNum, verseNum, numOfVerses;
   LookupResult result;
   if (argc < 4 || argc > 5){
      cerr << "Wrong Usage! Proper Usage : " << argv[0] << " 1 1 1 (1)" << endl;
      return 1;
   }
   
   bookNum = atoi(argv[1]);
   chapterNum = atoi(argv[2]); // cli implementation
   verseNum = atoi(argv[3]);

   // Create a reference from the numbers
   Ref ref(bookNum, chapterNum, verseNum);

   // Use the Bible object to retrieve the verse by reference
   cout << "Looking up reference: ";
   ref.display();
   cout << endl;

   verse = webBible.lookup(ref, result);
   
   if (result != SUCCESS){ //ensure success before attempting display
      cout << "Result status: " << webBible.error(result) << endl;
   }
   else {
      cout << "Verse Found! Result status: " << webBible.error(result) << endl;
      verse.display();
   }


   if(argc == 5){ //multiple verse implementation
      numOfVerses = atoi(argv[4]);

      for(int i = 1; i < numOfVerses; i++){
         verse = webBible.nextVerse(result);
         verse.display();
      }
   }
   cout << endl;
}