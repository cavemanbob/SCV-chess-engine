
#define BigTableSqNum 120
#define BitBoardSqNum 64

#define StartFen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short int u16;

struct board{
	u64 occupy;

	u64 wp;
	u64 wr;
	u64 wh;
	u64 wb;
	u64 wq;
	u64 wk;

	u64 bp;
	u64 br;
	u64 bh;
	u64 bb;
	u64 bq;
	u64 bk;

	u16 side;

	u16 castle; //  0000 0000 1111 black long castle short castle white long castle short castle
	u16 fifty;
	u16 movenum;
	u16 enpas;

	//note he defined a plist[13][10] that shows position of picies
};

int BigTable[BigTableSqNum];







