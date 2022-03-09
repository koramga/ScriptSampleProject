#pragma once

#include "ScriptGraph.h"
#include "ScriptGraphNode.h"
#include "ScriptGraphEdge.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "ScriptDesignerEditor.h"

#define LOG_INFO(FMT, ...) UE_LOG(LogTemp, Display, (FMT), ##__VA_ARGS__)
#define LOG_WARNING(FMT, ...) UE_LOG(LogTemp, Warning, (FMT), ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) UE_LOG(LogTemp, Error, (FMT), ##__VA_ARGS__)
