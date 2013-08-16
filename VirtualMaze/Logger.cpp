#include "Logger.h"

bool Logger::IsLoggingOn = false;


void Logger::Die(char *message, char *file, int line)
{
	char errbuf[512];

	if (file) {
		sprintf(errbuf, "Error (%s line %d): %s\n", file, line, message);
		message = errbuf;
	}

	/*size_t wn = mbsrtowcs(NULL, &message, 0, NULL);
	wchar_t * buf = new wchar_t[wn + 1]();  
	wn = mbsrtowcs(buf, &message, wn + 1, NULL);
	assert(message == NULL); */

	MessageBox(NULL, message, "DXQMAP", MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);	

	/*delete[] buf;*/

	ExitProcess(1); // Win32's exit()
}

void Logger::Log(char *s)
{
	if (!IsLoggingOn)
		return;

	FILE *fp;
	static int fatals;

	fp = fopen("game.log","a");
	if (fp)
	{
		if (strlen(s))
		{
			fprintf(fp,"%d: ",++fatals);
			fputs(s,fp);
			fputs("\n",fp);
		}
	}
	fclose(fp);
}