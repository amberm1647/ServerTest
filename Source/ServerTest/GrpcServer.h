// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
//#include "../../ThirdParty/include/grpcpp/grpcpp.h"
//#include "../../ThirdParty/include/grpcpp/health_check_service_interface.h"
//#include "../../ThirdParty/include/grpcpp/ext/proto_server_reflection_plugin.h"

#include "proto/helloworld.grpc.pb.h"

#include "GrpcServer.generated.h"

using helloworld::Greeter;


class SERVERTEST_API GrpcServer : public Greeter::Service
{
	
public:	
	// Sets default values for this actor's properties
	GrpcServer();


};
