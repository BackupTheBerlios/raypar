/////////////////////////////////////////////////////////
// Sample program to book                              //
//  Computer Graphics : Dynamics & Realistic Imaging.  //
//      by A.V. Boreskoff, E.V. Shikin                 //
//                                                     //
// Author:                                             //
//    Alex V. Boreskoff                                //
//                                                     //
// E-mail:                                             //
//    alex@garser.msk.su                               //
/////////////////////////////////////////////////////////

#include	"stdafx.h"
#include	<fcntl.h>
#include	<io.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h> 

#include	"targa.h"

TargaFile :: TargaFile ( char * name, int width, int height, char * comment )
{
	int write_size;  //number of bytes realy written on disk
	unlink ( name );			// remove file

	if (!(file = open ( name, O_CREAT | O_TRUNC | O_WRONLY, _S_IWRITE)))
	{
	    printf("Unable to create Targa file!\n");
	    exit(-1);
	}
	
	BufSize = 1000;
	Buffer  = new RGB [BufSize];
	pos     = 0;

	memset ( &Hdr, '\0', sizeof ( Hdr ) );

	Hdr.DataType = 2;
	Hdr.Width    = width;
	Hdr.Height   = height;
	Hdr.DataBits = 24;
	Hdr.ImType   = 32;

	if ( comment [0] != '\0' )
		Hdr.TextSize = strlen ( comment ) + 1;

	write_size = write ( file, &Hdr, sizeof ( Hdr ) );
	if (write_size!=sizeof(Hdr))
	{
	    printf("Write error!!!\n");
	    exit(-1);
	}

	if ( Hdr.TextSize > 0 )
		write ( file, comment, Hdr.TextSize );
}

TargaFile :: ~TargaFile ()
{
	if ( pos > 0 )
		Flush ();

	delete Buffer;
	close ( file );
}

void	TargaFile :: PutPixel ( RGB color )
{
	Buffer [pos].red   = color.blue;	// swap red & blue colors
	Buffer [pos].green = color.green;
	Buffer [pos].blue  = color.red;

	if ( ++pos >= BufSize )			// flush buffer if full
		Flush ();
}

void	TargaFile :: Flush ()
{
	write ( file, Buffer, pos * sizeof ( RGB ) );
	pos = 0;
}
