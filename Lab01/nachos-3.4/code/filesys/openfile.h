// openfile.h 
//	Data structures for opening, closing, reading and writing to 
//	individual files.  The operations supported are similar to
//	the UNIX ones -- type 'man open' to the UNIX prompt.
//
//	There are two implementations.  One is a "STUB" that directly
//	turns the file operations into the underlying UNIX operations.
//	(cf. comment in filesys.h).
//
//	The other is the "real" implementation, that turns these
//	operations into read and write disk sector requests. 
//	In this baseline implementation of the file system, we don't 
//	worry about concurrent accesses to the file system
//	by different threads -- this is part of the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef OPENFILE_H
#define OPENFILE_H

#include "copyright.h"
#include "utility.h"

#ifdef FILESYS_STUB			// Temporarily implement calls to 
					// Nachos file system as calls to UNIX!
					// See definitions listed under #else
class OpenFile {
  public:	
    //Khai bao bien type
    int type;
	//Constructor cua OpenFile
    OpenFile(int f) { file = f; currentOffset = 0; type=0; }	// open the file
	OpenFile(int f, int t) { file = f; currentOffset = 0; type = t; } // open the file with custom type
	//Destructor cua OpenFile
    ~OpenFile() { Close(file); }			// close the file

    int ReadAt(char *into, int numBytes, int position) { 
    		Lseek(file, position, 0); 
		return ReadPartial(file, into, numBytes); 
		}	
    int WriteAt(char *from, int numBytes, int position) { 
    		Lseek(file, position, 0); 
		WriteFile(file, from, numBytes); 
		return numBytes;
		}	
    int Read(char *into, int numBytes) {
		int numRead = ReadAt(into, numBytes, currentOffset); 
		currentOffset += numRead;
		return numRead;
    		}
    int Write(char *from, int numBytes) {
		int numWritten = WriteAt(from, numBytes, currentOffset); 
		currentOffset += numWritten;
		return numWritten;
		}
    int Seek(int pos) {
		Lseek(file, pos, 0);
		currentOffset = Tell(file);
		return currentOffset;
	}
    int Length() { 
	int len;
	Lseek(file, 0, 2); 
	len = Tell(file);
	Lseek(file, currentOffset, 0);
	return len; 
}
int GetCurrentPos() { currentOffset = Tell(file); return currentOffset; }
    
  private:
    int file;
    int currentOffset;
	int seekPosition;
};

#else // FILESYS
class FileHeader;

class OpenFile {
  public:
	//Khai bao bien type
    int type;
	// type 0 : read and write
	// type 1 : only read

    OpenFile(int sector);		// Open a file whose header is located
					// at "sector" on the disk
         OpenFile(int f, int t); // open the file with custom type

    ~OpenFile();			// Close the file

    void Seek(int position); 		// Set the position from which to 
					// start reading/writing -- UNIX lseek


    int Read(char *into, int numBytes); // Read/write bytes from the file,
					// starting at the implicit position.
					// Return the # actually read/written,
					// and increment position in file.
    int Write(char *from, int numBytes);

    int ReadAt(char *into, int numBytes, int position);
    					// Read/write bytes from the file,
					// bypassing the implicit position.
    int WriteAt(char *from, int numBytes, int position);

    int Length(); 			// Return the number of bytes in the
					// file (this interface is simpler 
					// than the UNIX idiom -- lseek to 
					// end of file, tell, lseek back 
    int GetCurrentPos()
	{
		return seekPosition;
	}
	
  private:
    FileHeader *hdr;			// Header for this file 
    int seekPosition;			// Current position within the file
};

#endif // FILESYS

#endif // OPENFILE_H
