#pragma once

#include "AuctionParser.h"
#include "AuctionSimpleFile.h"
#include "AuctionWriter.h"
#include "Core/BackgroundWork.h"
#include "Core/Object.h"
#include "IPipeline.h"
#include "PipelineState.h"

class P2DeserializeAuction : public PipelineStep<std::pair<PipelineState, std::vector<AuctionWriter::file_data_byte>>,
                                                 std::pair<PipelineState, AUCTION_SIMPLE_FILE>> {
	DECLARE_CLASSNAME(P2DeserializeAuction, 0)
public:
	P2DeserializeAuction();
	virtual void doWork(std::shared_ptr<WorkItem> item) override;
	virtual void doCancelWork(std::shared_ptr<WorkItem> item) override { work.cancel(); }

private:
	int processWork(std::shared_ptr<WorkItem> item);
	void afterWork(std::shared_ptr<WorkItem> item, int status);

private:
	BackgroundWork<P2DeserializeAuction, std::shared_ptr<WorkItem>> work;
};
