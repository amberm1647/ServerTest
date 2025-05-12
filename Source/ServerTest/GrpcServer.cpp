// Fill out your copyright notice in the Description page of Project Settings.


#include "GrpcServer.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;


// Sets default values
GrpcServer::GrpcServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}


Status GrpcServer::SayHello(ServerContext* context, const HelloRequest* request,
    HelloReply* reply) override {
    std::string prefix("Hello ");
    std::cout << "Hello 2" << std::endl;
    reply->set_message(prefix + request->name());
    return Status::OK;
}