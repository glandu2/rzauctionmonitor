#include "AuctionParser.h"
#include "AuctionPipeline.h"
#include "AuctionSQLWriter.h"
#include "Console/ConsoleSession.h"
#include "Core/CrashHandler.h"
#include "Core/EventLoop.h"
#include "Core/Log.h"
#include "Database/DbBindingLoader.h"
#include "Database/DbConnectionPool.h"
#include "GlobalConfig.h"
#include "LibRzuInit.h"
#include "NetSession/ServersManager.h"
#include "SqlWriter.h"

static void onTerminate(void* instance) {
	ServersManager* serverManager = (ServersManager*) instance;

	if(serverManager) {
		serverManager->forceStop();
	}
}

int main(int argc, char* argv[]) {
	LibRzuScopedUse useLibRzu;
	GlobalConfig::init();

	DbConnectionPool dbConnectionPool;
	DbBindingLoader::get()->initAll(&dbConnectionPool);
	PipelineStepMonitor::init();

	ConfigInfo::get()->init(argc, argv);

	Log mainLogger(GlobalCoreConfig::get()->log.enable,
	               GlobalCoreConfig::get()->log.level,
	               GlobalCoreConfig::get()->log.consoleLevel,
	               GlobalCoreConfig::get()->log.dir,
	               GlobalCoreConfig::get()->log.file,
	               GlobalCoreConfig::get()->log.maxQueueSize);
	Log::setDefaultLogger(&mainLogger);

	ConfigInfo::get()->dump();

	ServersManager serverManager;
	//	SqlWriter sqlWriter;
	//	AuctionParser auctionParser(&sqlWriter,
	//	                            CONFIG_GET()->input.auctionsPath,
	//	                            CONFIG_GET()->input.changeWaitSeconds,
	//	                            CONFIG_GET()->states.statesPath,
	//	                            CONFIG_GET()->states.auctionStateFile,
	//	                            CONFIG_GET()->states.aggregationStateFile);

	AuctionPipeline auctionParser(CONFIG_GET()->input.auctionsPath,
	                              CONFIG_GET()->input.changeWaitSeconds,
	                              CONFIG_GET()->states.statesPath,
	                              CONFIG_GET()->states.auctionStateFile);

	serverManager.addServer("auction.monitor", &auctionParser, nullptr);

	ConsoleServer consoleServer(&serverManager);
	// DB_Item::createTable(&dbConnectionPool);
	serverManager.start();

	CrashHandler::setTerminateCallback(&onTerminate, &serverManager);
	EventLoop::getInstance()->run(UV_RUN_DEFAULT);
	CrashHandler::setTerminateCallback(nullptr, nullptr);

	return 0;
}
