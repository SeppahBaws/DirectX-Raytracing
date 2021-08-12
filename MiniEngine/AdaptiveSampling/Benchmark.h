#pragma once

#include "Math/Vector.h"

struct BenchConfig
{
	Math::Vector3 cameraPos;
	float cameraPitch;
	float cameraYaw;
	Math::Vector3 shadowDir;
	float shadowSphereSize;
	float shadowSphereOffset;
};

class Benchmark
{
public:
	using Callback = std::function<void(const BenchConfig&)>;
	using Debug = std::function<void()>;

public:
	Benchmark() = default;

	void Update(float deltaT);

	void SelectConfig(int idx);
	void NextConfig();
	void RegisterCallback(Callback callback);

	void OutputDebug();
	void RegisterDebug(Debug debug);

	void Load(const std::string& filepath);

	[[nodiscard]] std::string GetSceneFile() const { return m_SceneFile; }

	void DumpCSV();

private:
	std::string m_SceneFile{};
	std::vector<BenchConfig> m_Configs{};

	int m_ConfigIdx{ 0 };
	std::vector<Callback> m_Callbacks{};

	std::vector<Debug> m_Debugs{};

	static constexpr uint32_t FRAMES_COUNT = 1000;
	uint32_t m_FramesCounted{ FRAMES_COUNT + 1 };
	float m_Timings[FRAMES_COUNT]{};
};
