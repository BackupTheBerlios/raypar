//////////////////////////////////////////////////////////////////////////
// Basic class for writing to an uncimpressed 24-bit targa file ( TGA ) //
// Author: Alex V. Boreskoff                                            //
// Last revision: 3/12/94                                               //
//////////////////////////////////////////////////////////////////////////

#ifndef	__TARGA__
#define	__TARGA__

#pragma pack(1)
struct	TargaHeader
{
	char	TextSize;
	char	MapType;
	char	DataType;
	short	MapOrg;
	short	MapLength;
	char	CMapBits;
	short	XOffset;
	short	YOffset;
	short	Width;
	short	Height;
	char	DataBits;
	char	ImType;
};
#ifndef	__RGB__
#define	__RGB__
#pragma pack(1)
struct	RGB
{
	char	red;
	char	green;
	char	blue;
};
#endif

class	TargaFile		// basic class for writing TGA image files
{
public:
	TargaFile ( char *, int, int, char * = "" );
	~TargaFile ();

	void	PutPixel ( RGB );
private:
	TargaHeader	Hdr;
	RGB	      * Buffer;
	int		BufSize;
	int		pos;
	int		file;

	void		Flush ();
};

#endif
