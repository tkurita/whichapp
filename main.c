#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>
#include <getopt.h>

#define useLog 0

#define	kMaxPathLength 4096

void usage() {
	printf("Usage: whichapp [-t] [Creator Type] [-i] [Bundle Identifier] [File Name]\n");
}

void showHelp() {
	usage();
	printf("\nlocate an application with given infomation.\n");
	printf("The application is specified with following parameters.\n\n");
	printf("File Name         -- File name of application. The extention is required.\n");
	printf("Creator Type      -- The value of CFBundleSignagure of an application bundle.\n");
	printf("Bundle Identifier -- The value of CFBundleIdentifier of an application bundle.\n");
}

void showVersion() {
	printf("whichapp 1.0.1 copyright 2005-2016, Tetsuro KURITA\n");
}

int main (int argc, char * const argv[]) {
	CFURLRef outAppURL;
	FSRef outAppRef;
	CFStringRef inBundleID = NULL;
	CFStringRef inName = NULL;
	OSType inCreator =  kLSUnknownCreator;
	
	static struct option long_options[] = {
		{"version", no_argument, NULL, 'v'},	
		{"help", no_argument, NULL, 'h'},
		{"identifier",required_argument, NULL, 'i'},
		{"type",required_argument,NULL,'t'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	
	int c;
	CFStringRef theCreator;
	
	if (argc <= 1) {
		printf("No arguments.\n");
		showHelp();
		return 1;
	}
	
	while(1){
		c = getopt_long(argc, argv, "t:i:vh",long_options, &option_index);
        if (c == -1)
            break;
		
		switch(c){			
			case 'h':
				showHelp();
				exit(0);
			case 'v':
				showVersion();
				exit(0);
			case 't':
				theCreator = CFStringCreateWithCString(NULL,optarg,kCFStringEncodingUTF8);
				inCreator = UTGetOSTypeFromString(theCreator);
				break;
			case 'i': 
				inBundleID = CFStringCreateWithCString(NULL,optarg,kCFStringEncodingUTF8);
				break;				
			case '?':
			default	:
				printf("There is unknown option.\n");
				usage(); 
				exit(-1);
				break;
		}
		optarg	=	NULL;
	}
	
#if useLog
	printf("%i\t%i\n",argc,optind);
#endif
	
	if (optind < argc) {		
		inName = CFStringCreateWithCString(NULL,argv[optind],kCFStringEncodingUTF8);
	}

	
	OSStatus resultStatus = LSFindApplicationForInfo(inCreator,inBundleID,inName,&outAppRef,&outAppURL);

	if (resultStatus != 0) {
		printf("The application is fot found. result status : %d\n",(int)resultStatus);
		exit(1);
	}
	
	//CFShow(CFURLGetString(outAppURL));
	UInt8 appPath[kMaxPathLength];
	
	resultStatus = FSRefMakePath(&outAppRef,appPath,sizeof(appPath));

	printf("%s\n",appPath);
//	if (optind < argc) {		
//		targetName = [NSString stringWithCString:argv[optind]];
//	}
	
    return 0;
}
