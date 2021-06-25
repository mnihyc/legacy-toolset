#include "main.h"
#pragma comment(lib,"winmm.lib")

int main(int argc, char**argv)
{
	ChangeColor(COLOR_WHITE);
	system("cls");//For easy
	DebugLevel = ALL_DEBUG;

	if (argc <= 1){ usage(); getch(); return 99999; }
	/*wchar_t*/char FileName[MAX_PATH] = { 0 };//No chinese support
	strcpy_s(FileName, argv[1]);
	if (!FileExists(FileName))DEBUG::err("The wave file is not exists !");
	DEBUG::info("The wave file is exists !");
	bool HasCommand = false, HasOption = false;
	if (argc >= 3)HasCommand = true;
	if (argc >= 4)HasOption = true;
	USHORT Command = COMMAND_NOFOUND;
	if (HasCommand)
	{
		if (StrLowEqu(argv[2], "pack"))Command = COMMAND_PACK;
		if (StrLowEqu(argv[2], "unpack"))Command = COMMAND_UNPACK;
		if (StrLowEqu(argv[2], "draw"))Command = COMMAND_DRAW_PICTURE;
		if (StrLowEqu(argv[2], "play"))Command = COMMAND_PLAY_WAVE;
		if (StrLowEqu(argv[2], "to8bit"))Command = COMMAND_CONVERT_TO8BIT;
		if (StrLowEqu(argv[2], "out"))Command = COMMAND_DEBUG_OUTSOUND;
		if (StrLowEqu(argv[2], "addvol"))Command = COMMAND_ADDVOLUME;
		if (Command == COMMAND_NOFOUND)DEBUG::err("No found command like '" + (string)argv[2] + "' !");
		DEBUG::info("Command '" + (string)argv[2] + "' is exists !");
	}
	else
	{
		if (CompStr(FileName, ".wav"))Command = COMMAND_DRAW_PICTURE;//???
		if (CompStr(FileName, PACK_FILEEXT))Command = COMMAND_UNPACK;
		if (Command == COMMAND_NOFOUND)DEBUG::err("Can't set the command autoly !\nPlease visit usage !");
		DEBUG::wrn("Set the command to '" + (string)(Command == COMMAND_DRAW_PICTURE ? "DRAW" : (Command == COMMAND_UNPACK ? "UnPACK" : "WTF?")) + "' autoly !");
	}
	char*OutputFileName = new char[MAX_PATH];
	ZeroMemory(OutputFileName, MAX_PATH);
	bool JumpOffCheck = false;
	int RepeatPlay = 1;
	double AddVolumeNum = 0;
	if (HasOption)
	{
		for (int i = 3; i < argc; i += 1)
		{
			if (StrLowEqu(argv[i], "-o"))
			{
				if (!(i + 1 <= argc))DEBUG::err("The number of the ARG is wrong at the PARA '-o' !");
				DEBUG::info("Check the PARA '-o' with '" + (string)argv[i + 1] + "'");
				strncpy(OutputFileName, argv[i + 1], MAX_PATH);
				i++;
				continue;
			}
			if (StrLowEqu(argv[i], "-noload")||StrLowEqu(argv[i],"-withoutload"))
			{
				DEBUG::info("Check the PARA '-noload'");
				JumpOffCheck = true;
				if (Command != COMMAND_PLAY_WAVE)DEBUG::err("Error at PARA '-noload' because the command need to load the wave file !");
				continue;
			}
			if (StrLowEqu(argv[i], "-repeat"))
			{
				if (!(i + 1 <= argc))DEBUG::err("The number of the ARG is wrong at the PARA '-repeat' !");
				DEBUG::info("Check the PARA '-repeat' with '" + (string)argv[i + 1] + "'");
				INT tmp = atoi(argv[i + 1]);
				if (tmp == 0 || tmp < -1){ DEBUG::wrn("Error at PARA '-repeat' [number>0 or number=-1] !\nSet the number to -1 autoly !"); tmp = -1; }
				RepeatPlay = tmp;
				i++;
				continue;
			}
			if (StrLowEqu(argv[i], "-vol"))
			{
				if (!(i + 1 <= argc))DEBUG::err("The number of the ARG is wrong at the PARA '-vol' !");
				DEBUG::info("Check the PARA '-vol' with '" + (string)argv[i + 1] + "'");
				long tmp = atof(argv[i + 1]);
				if (tmp>32767 || tmp < -32768)DEBUG::err("Error at PARA '-vol' [+ or - number(-32768<N<32767) ] !");
				if (tmp == 0)DEBUG::err("WTF do you want to do ?");
				AddVolumeNum = tmp;
				i++;
				continue;
			}

		}
	}
	if (OutputFileName[0] == '\0')
	{
		strcpy(OutputFileName, FileName);
		char**tmp = NULL; tmp = &OutputFileName;
		if (Command == COMMAND_PACK)
		{
			ChangeFileExt(tmp, PACK_FILEEXT);
		}
		if (Command == COMMAND_UNPACK)
		{
			ChangeFileExt(tmp, ".wav");
		}
	}
	DEBUG::info("Apply that the FullPath of OutputFile is '" + (string)OutputFileName + "'");
	if (JumpOffCheck)goto start_cmd;
	if (Command == COMMAND_ADDVOLUME&&AddVolumeNum == 0)DEBUG::err("Command=AddVol but not found the PARA -vol [number] !");
	ReadWavePackage(FileName);
	DEBUG::info("Load the wave file successflly ! ---> ");
	printf("\tFileSize: %ld bytes = %.3lf Kbytes = %.6lf Mbytes\n", Header->dwRiffSize + 8, (Header->dwRiffSize + 8) / 1024.0, (Header->dwRiffSize + 8) / 1024.0/1024.0);
	cout << "\tChannel: " << Sound->channel << "" << endl;
	cout << "\tBits: " << Sound->BitsPerSample << " bit" << endl;
	cout << "\tFREQ: " << Fmt->wavFormat.dwSamplesPerSec << " Hz" << endl;
	cout << "\tBytes need every second: " << Fmt->wavFormat.dwAvgBytesPerSec << "" << endl;
	printf("\tTime length: %.6lf sec = %.6lf min = %ldmin%ldsec\n",Sound->TimeLast,Sound->TimeLast/60.0,(long)floor(Sound->TimeLast/60.0),(long)((Sound->TimeLast/60.0-floor(Sound->TimeLast/60.0))*60));
	cout << endl;
start_cmd:
	if (Command == COMMAND_DRAW_PICTURE)
	{
		DrawPicture(FileName);
	}
	else if (Command == COMMAND_PLAY_WAVE){ for (int i = 1; i <= RepeatPlay||RepeatPlay==-1;i++)sndPlaySoundA(FileName, SND_SYNC); }
	else if (Command == COMMAND_CONVERT_TO8BIT)
	{
		FILE*fp = fopen(OutputFileName, "wb");
		if (fp == NULL)DEBUG::err("Can't open the OutputFile for writing !");
		bool CopySuc = true;
		if (Fmt->wavFormat.wBitsPerSample == 8)DEBUG::wrn("This wave sound is 8 bit !\nOutput the source file.");
		else
		{
			Fmt->wavFormat.wBitsPerSample =8 ;
			Data->dwDataSize /= 2;
			Fmt->wavFormat.dwAvgBytesPerSec /= 2;
			Fmt->wavFormat.wBlockAlign = Fmt->wavFormat.wChannels;
		}
		cout << "Writing the wave file ... ";
		if (Sound->BitsPerSample == 8){ CopySuc=CopyFileA(FileName,OutputFileName,false); goto writeend; }
		size_t opt;
		opt=fwrite((char*)Header, sizeof(*Header), 1, fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Header !"); }
		opt=fwrite((char*)Fmt,sizeof(*Fmt),1,fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Fmt !"); }
		opt=fwrite((char*)Data,sizeof(*Data),1,fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Data !"); }
		BYTE b;
		for (int i = 0; i < Sound->num / Sound->BytesPerSample; i++)
		{
			b = HighPart(Sound->ldata[i]) +128;
			opt=fwrite((char*)&b,sizeof(b),1,fp);
			if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
			if (Sound->channel == 2)
			{
				b = HighPart(Sound->rdata[i]) +128;
				opt=fwrite((char*)&b, sizeof(b), 1, fp);
				if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
			}
		}
		long pos = ftell(fp);
		Header->dwRiffSize = pos - 8;
		fseek(fp, 0, SEEK_SET);
		opt=fwrite((char*)Header, sizeof(*Header), 1, fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Header *2 !"); }
writeend:
		if (CopySuc){ ChangeColor(COLOR_GREEN); cout << "OK.\n"; ChangeColor(COLOR_WHITE); }
		else{ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when copying the file !"); }
		DEBUG::info("Write successfully !");
		fclose(fp);
	}
	else if (Command == COMMAND_DEBUG_OUTSOUND){ getch(); for (int i = 0; i < Sound->num / Sound->BytesPerSample; i++){ cout << Sound->ldata[i]; if (Sound->channel == 2)cout << " " << Sound->rdata[i]; cout << "\t"; } }
	else if (Command == COMMAND_ADDVOLUME)
	{
		FILE*fp = fopen(OutputFileName, "wb");
		if (fp == NULL)DEBUG::err("Can't open the OutputFile for writing !");
		cout << "Writing the wave file ... ";
		size_t opt;
		opt = fwrite((char*)Header, sizeof(*Header), 1, fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Header !"); }
		opt = fwrite((char*)Fmt, sizeof(*Fmt), 1, fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Fmt !"); }
		opt = fwrite((char*)Data, sizeof(*Data), 1, fp);
		if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Block Data !"); }
		BYTE b; BYTE c[2] = { 0 }; long tmp;
		for (int i = 0; i < Sound->num / Sound->BytesPerSample; i++)
		{
			if (Sound->BytesPerSample == 1)
			{
				b = (HighPart(Sound->ldata[i]) + 128)*AddVolumeNum;
				if (b < 0)b = 0; if (b>255)b = 255;
				opt = fwrite((char*)&b, sizeof(b), 1, fp);
				if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
				if (Sound->channel == 2)
				{
					b = (HighPart(Sound->rdata[i]) + 128)*AddVolumeNum;
					if (b < 0)b = 0; if (b>255)b = 255;
					opt = fwrite((char*)&b, sizeof(b), 1, fp);
					if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
				}
			}
			else
			{
				tmp = Sound->ldata[i]*AddVolumeNum;
				if (tmp < -32768)tmp = -32768; if (tmp>32767)tmp = 32767;
				c[0] = LowPart(tmp); c[1] = HighPart(tmp);
				opt = fwrite((char*)&c, sizeof(c), 1, fp);
				if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
				if (Sound->channel == 2)
				{
					tmp = Sound->rdata[i] * AddVolumeNum;
					if (tmp < -32768)tmp = -32768; if (tmp>32767)tmp = 32767;
					c[0] = LowPart(tmp); c[1] = HighPart(tmp);
					opt = fwrite((char*)&c, sizeof(c), 1, fp);
					if (opt != 1){ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); DEBUG::err("Error when writing the Sound Datas !"); }
				}
			}
		}
		ChangeColor(COLOR_GREEN); cout << "OK.\n"; ChangeColor(COLOR_WHITE); 
		DEBUG::info("Write successfully !");
		fclose(fp);
	}
	else{ DEBUG::err("WTF?"); }


}