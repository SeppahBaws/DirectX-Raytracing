//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#pragma once

#include <string>
#include <unordered_map>


#include "GraphRenderer.h"
#include "TextRenderer.h"
#include <SystemTime.h>

#include "CommandContext.h"
#include "GpuTimeManager.h"

class CommandContext;

namespace EngineProfiling
{
    static bool Paused = false;

	void Update();

    void BeginBlock(const std::wstring& name, CommandContext* Context = nullptr);
    void EndBlock(CommandContext* Context = nullptr);

    void DisplayFrameRate(TextContext& Text);
    void DisplayPerfGraph(GraphicsContext& Text);
    void Display(TextContext& Text, float x, float y, float w, float h);
    bool IsPaused();
}

#ifdef RELEASE
class ScopedTimer
{
public:
    ScopedTimer(const std::wstring&) {}
    ScopedTimer(const std::wstring&, CommandContext&) {}
};
#else
class ScopedTimer
{
public:
	ScopedTimer(const std::wstring& name);

	ScopedTimer(const std::wstring& name, CommandContext& Context);

	~ScopedTimer();

private:
    CommandContext* m_Context;
};
#endif

#define PERF_GRAPH_ERROR uint32_t(0xFFFFFFFF)

class StatHistory
{
public:
	StatHistory();

	void RecordStat(uint32_t FrameIndex, float Value);

	float GetLast(void) const;
	float GetMax(void) const;
	float GetMin(void) const;
	float GetAvg(void) const;

	const float* GetHistory(void) const;
	uint32_t GetHistoryLength(void) const;

private:
    static const uint32_t kHistorySize = 64;
    static const uint32_t kExtendedHistorySize = 256;
    float m_RecentHistory[kHistorySize];
    float m_ExtendedHistory[kExtendedHistorySize];
    float m_Recent;
    float m_Average;
    float m_Minimum;
    float m_Maximum;
};

class GpuTimer
{
public:

    GpuTimer::GpuTimer()
    {
        m_TimerIndex = GpuTimeManager::NewTimer();
    }

    void Start(CommandContext& Context);

    void Stop(CommandContext& Context);

    float GpuTimer::GetTime(void)
    {
        return GpuTimeManager::GetTime(m_TimerIndex);
    }

    uint32_t GetTimerIndex(void);
private:

    uint32_t m_TimerIndex;
};

class NestedTimingTree
{
public:
	NestedTimingTree(const std::wstring& name, NestedTimingTree* parent = nullptr);

	NestedTimingTree* GetChild(const std::wstring& name);

	NestedTimingTree* NextScope(void);

	NestedTimingTree* PrevScope(void);

	NestedTimingTree* FirstChild(void);

	NestedTimingTree* LastChild(void);

	NestedTimingTree* NextChild(NestedTimingTree* curChild);

	NestedTimingTree* PrevChild(NestedTimingTree* curChild);

	void StartTiming(CommandContext* Context);

	void StopTiming(CommandContext* Context);

	void GatherTimes(uint32_t FrameIndex);

	void SumInclusiveTimes(float& cpuTime, float& gpuTime);

    static void PushProfilingMarker(const std::wstring& name, CommandContext* Context);
    static void PopProfilingMarker(CommandContext* Context);
    static void Update(void);
	static void UpdateTimes(void);

	static float GetTotalCpuTime(void);
	static float GetTotalGpuTime(void);
	static float GetFrameDelta(void);

	static void Display(TextContext& Text, float x);

	void Toggle();
	bool IsGraphed();

private:

    void DisplayNode(TextContext& Text, float x, float indent);
    void StoreToGraph(void);
	void DeleteChildren(void);

    std::wstring m_Name;
    NestedTimingTree* m_Parent;
    std::vector<NestedTimingTree*> m_Children;
    std::unordered_map<std::wstring, NestedTimingTree*> m_LUT;
    int64_t m_StartTick;
    int64_t m_EndTick;
    StatHistory m_CpuTime;
    StatHistory m_GpuTime;
    bool m_IsExpanded;
    GpuTimer m_GpuTimer;
    bool m_IsGraphed;
    GraphRenderer::GraphHandle m_GraphHandle;
    static StatHistory s_TotalCpuTime;
    static StatHistory s_TotalGpuTime;
    static StatHistory s_FrameDelta;
    static NestedTimingTree sm_RootScope;
    static NestedTimingTree* sm_CurrentNode;
    static NestedTimingTree* sm_SelectedScope;

    static bool sm_CursorOnGraph;

};
