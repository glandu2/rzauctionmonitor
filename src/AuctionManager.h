#pragma once

#include "AuctionSimpleDiffWriter.h"
#include "AuctionWorker.h"
#include "Core/Object.h"
#include "NetSession/StartableObject.h"
#include <deque>
#include <memory>
#include <time.h>

struct TS_SC_CHARACTER_LIST;
struct TS_SC_LOGIN_RESULT;
struct TS_SC_AUCTION_SEARCH;
class IWritableConsole;

class AuctionManager : public Object, public IListener, public StartableObject {
	DECLARE_CLASS(AuctionManager)

public:
	AuctionManager();

	bool start();
	void stop();
	bool isStarted();

	void reloadAccounts();
	void loadAccounts();

	std::unique_ptr<AuctionWorker::AuctionRequest> getNextRequest();
	void addAuctionInfo(
	    const AuctionWorker::AuctionRequest* request, uint32_t uid, uint32_t epic, const uint8_t* data, int len);
	void onAuctionSearchCompleted(bool success,
	                              int pageTotal,
	                              std::unique_ptr<AuctionWorker::AuctionRequest> auctionRequest);

	static void onReloadAccounts(IWritableConsole* console, const std::vector<std::string>& args);

private:
	void stopClients();
	static void onClientStoppedStatic(IListener* instance, AuctionWorker* worker);
	void onClientStopped(AuctionWorker* worker);
	void onAccountReloadTimer();

	void addRequest(int category, int page);

	bool isAllRequestProcessed();
	void onAllRequestProcessed();
	void dumpAuctions();

	void loadInitialState();

private:
	AuctionSimpleDiffWriter auctionWriter;
	std::vector<std::unique_ptr<AuctionWorker>> clients;
	std::vector<std::unique_ptr<AuctionWorker>> stoppingClients;

	std::deque<std::unique_ptr<AuctionWorker::AuctionRequest>> pendingRequests;

	bool stopped;
	int totalPages;
	static const int CATEGORY_MAX_INDEX = 19;
	int currentCategory;
	bool firstDump;
	time_t firstDumpTime;

	bool reloadingAccounts;
	Timer<AuctionManager> accountReloadTimer;
	static AuctionManager* instance;
	std::vector<uint8_t> fileData;  // cache allocated memory
};

