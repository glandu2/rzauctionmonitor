#pragma once

#include "Core/BackgroundWork.h"
#include "Core/Object.h"
#include "IPipeline.h"
#include "PipelineState.h"
#include <stdint.h>

class AuctionPipeline;

class P5Commit : public PipelineStep<PipelineState, void> {
	DECLARE_CLASSNAME(P5Commit, 0)
public:
	P5Commit(AuctionPipeline* auctionPipeline);
	virtual void doWork(std::shared_ptr<WorkItem> item) override;
	virtual void doCancelWork(std::shared_ptr<WorkItem> item) override { work.cancel(); }

private:
	int processWork(std::shared_ptr<WorkItem> item);
	void afterWork(std::shared_ptr<WorkItem> item, int status);

private:
	AuctionPipeline* auctionPipeline;
	BackgroundWork<P5Commit, std::shared_ptr<WorkItem>> work;
};
