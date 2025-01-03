#include <iostream>
#include <fstream>
#include <string>
#include "fwlib32.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define  USHORT unsigned short
#define  MAXLEN 1280

using namespace std;

short DownloadFile (USHORT h, const char *filepath);
short UploadFile (USHORT h, short progID, const char *filePath);
void HexDump (char *buf, int len, int addr);

int
main (int argc, char **argv)
{
  unsigned short h = 1;
  short ret;
  string ip = "180.169.31.34";
  short port = 18193;

  if (argc > 1)
    {
      ip = argv[1];
    }

  if (EW_OK != cnc_startupprocess (0, "/var/log/foces32.log"))
    {
      printf ("statup proceess fialed");
      return -1;
    }

  cout << "machine: " << ip << endl;
  if (EW_OK != cnc_allclibhndl3 (ip.c_str (), port, 5, &h))	/// Allocate the library handle(for Ethernet connection)
    {
      printf ("Connect Failed!\n");
      system ("Pause");
      return -1;
    }


  ODBST statinfo;
  memset ((void *) &statinfo, 0, sizeof (statinfo));
  ret = cnc_statinfo (h, &statinfo);

  if (EW_OK == ret)
    {
      cout << "AUTOMATIC/MANUAL mode selection \t:" << statinfo.aut << endl;
      cout << "Status of automatic operation \t\t: " << statinfo.run << endl;
      cout << "Status of axis movement, dwell \t\t: " << statinfo.motion <<
	endl;
      cout << "Status of M,S,T,B function \t\t: " << statinfo.mstb << endl;
      cout << "Status of alarm \t\t: " << statinfo.alarm << endl;
      cout << "Status of program editing \t\t: " << statinfo.edit << endl;
    }

  ODBSYS sysinfo;
  memset ((void *) &sysinfo, 0, sizeof (sysinfo));
  ret = cnc_sysinfo (h, &sysinfo);
  if (EW_OK == ret)
    {
      const int STR_MAX = 32;
      char cnc_type[STR_MAX] = { 0 };
      char mt_type[STR_MAX] = { 0 };
      char series[STR_MAX] = { 0 };
      char version[STR_MAX] = { 0 };
      memcpy (cnc_type, sysinfo.cnc_type, 2 * sizeof (char));
      memcpy (mt_type, sysinfo.mt_type, 2 * sizeof (char));
      memcpy (series, sysinfo.series, 4 * sizeof (char));
      memcpy (version, sysinfo.version, 4 * sizeof (char));
      cout << "cnc type:" << cnc_type << endl;
      cout << "mt type\t:" << mt_type << endl;
      cout << "series\t:" << series << endl;
      cout << "version\t:" << version << endl;
    }

  cout << "alarm===================" << endl;
  {
    ODBALMMSG msg[10];
    short rnum = 10;

    bool isOk = false;
    if (cnc_rdalmmsg ((unsigned short) h, -1, &rnum, msg) == EW_OK)
      {
	for (short i = 0; i < rnum; i++)
	  {
	    msg[i].alm_msg[31] = '\0';
	    cout << "ID_" << i << ": " << msg[i].alm_no << ", type: " <<
	      msg[i].type << endl;
	    cout << "Msg: " << msg[i].alm_msg << endl;
	    HexDump (msg[i].alm_msg, msg[i].msg_len, 0);
	  }
	isOk = true;
      }
  }


void
HexDump (char *buf, int len, int addr)
{
  int i, j, k;
  char binstr[80];

  for (i = 0; i < len; i++)
    {
      if (0 == (i % 16))
	{
	  sprintf (binstr, "%08x -", i + addr);
	  sprintf (binstr, "%s %02x", binstr, (unsigned char) buf[i]);
	}
      else if (15 == (i % 16))
	{
	  sprintf (binstr, "%s %02x", binstr, (unsigned char) buf[i]);
	  sprintf (binstr, "%s  ", binstr);
	  for (j = i - 15; j <= i; j++)
	    {
	      sprintf (binstr, "%s%c", binstr,
		       ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
	    }
	  printf ("%s\n", binstr);
	}
      else
	{
	  sprintf (binstr, "%s %02x", binstr, (unsigned char) buf[i]);
	}
    }
  if (0 != (i % 16))
    {
      k = 16 - (i % 16);
      for (j = 0; j < k; j++)
	{
	  sprintf (binstr, "%s   ", binstr);
	}
      sprintf (binstr, "%s  ", binstr);
      k = 16 - k;
      for (j = i - k; j < i; j++)
	{
	  sprintf (binstr, "%s%c", binstr,
		   ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
	}
      printf ("%s\n", binstr);
    }
}
