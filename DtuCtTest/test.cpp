#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
//#include "DataFromFile.h"
#include "wifiRvData.h"
#include "ProcessStData.h"
#include "ProcessRvData.h"

#include "head_info.h"
#include "wcomm_dll.h"
//#pragma comment(lib,"winmm.lib")


int main(int argc,char *argv[])
{
	int	Terminated	= 0;
	data_record		drToRec	=	{ USERID,"","",0,0};
	user_info		ui		=	{ USERID,0};	

	MyQueue			dataStoreQueue;
	

	initialize();


	ProcessRvData	prd( &drToRec,&ui,&dataStoreQueue );	//receive the DTU data
	//DataFromFile	pdf( &drToRec,&ui,&dataStoreQueue );	//test from the file data
	ProcessStData	psd( &dataStoreQueue );
	wifiRvData		pwd( &dataStoreQueue );
	boost::thread	thrdRv( boost::bind( &ProcessRvData::ReceiveData,&prd,&Terminated ) );
	boost::thread	thrdWifiRv( boost::bind( &wifiRvData::ReceiveData,&pwd ) );
	boost::thread	thrdSt( boost::bind( &ProcessStData::readData,&psd,&Terminated ) );

	char	tFlag;
	std::cout<<"please enter q to stop"<<std::endl;
	std::cin>>tFlag;
	while( tFlag!='q' )
	{
		std::cout<<"unknown signal"<<std::endl;
		std::cin>>tFlag;
	}
	Terminated	=	1;

	stopService();
	pwd.stop();
	thrdWifiRv.join();
	thrdSt.join();

	return	0;
}

