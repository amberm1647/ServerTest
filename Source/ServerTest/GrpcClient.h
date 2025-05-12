// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "proto/helloworld.grpc.pb.h"
#include "GrpcClient.generated.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;


class SERVERTEST_API GrpcClient : stub_(Greeter::NewStub(channel))
{
	
public:	
	// Sets default values for this actor's properties
	GrpcClient(std::shared_ptr<Channel> channel);


private:
	std::unique_ptr<Greeter::Stub> stub_;

};
