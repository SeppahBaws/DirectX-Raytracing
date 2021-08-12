#include "pch.h"
#include "Benchmark.h"

#include <fstream>
#include <iostream>
#include <ctime>

#include "Dependencies/json.hpp"

namespace nlohmann
{
	template<>
	struct adl_serializer<Math::Vector3>
	{
		static void to_json(json& j, const Math::Vector3& v)
		{
			j = json::array({
				static_cast<float>(v.GetX()),
				static_cast<float>(v.GetY()),
				static_cast<float>(v.GetZ())
			});
		}

		static void from_json(const json& j, Math::Vector3& v)
		{
			std::vector<float> values;
			j.get_to(values);

			v.SetX(values[0]);
			v.SetY(values[1]);
			v.SetZ(values[2]);
		}
	};

	template<>
	struct adl_serializer<Math::Vector4>
	{
		static void to_json(json& j, const Math::Vector4& v)
		{
			j = json::array({
				static_cast<float>(v.GetX()),
				static_cast<float>(v.GetY()),
				static_cast<float>(v.GetZ()),
				static_cast<float>(v.GetW())
				});
		}

		static void from_json(const json& j, Math::Vector4& v)
		{
			std::vector<float> values;
			j.get_to(values);

			v.SetX(values[0]);
			v.SetY(values[1]);
			v.SetZ(values[2]);
			v.SetW(values[3]);
		}
	};

	template<>
	struct adl_serializer<BenchConfig>
	{
		static void to_json(json& j, const BenchConfig& c)
		{
			j = json::object();
			j["cameraPos"] = c.cameraPos;
			j["cameraPitch"] = c.cameraPitch;
			j["cameraYaw"] = c.cameraYaw;
			j["shadowDir"] = c.shadowDir;
			j["shadowSphereSize"] = c.shadowSphereSize;
			j["shadowSphereOffset"] = c.shadowSphereOffset;
		}

		static void from_json(const json& j, BenchConfig& c)
		{
			c.cameraPos = j["cameraPos"];
			c.cameraPitch = j["cameraPitch"];
			c.cameraYaw = j["cameraYaw"];
			c.shadowDir = j["shadowDir"];
			c.shadowSphereSize = j["shadowSphereSize"];
			c.shadowSphereOffset = j["shadowSphereOffset"];
		}
	};
}

void Benchmark::Update(float /*deltaT*/)
{
	if (m_FramesCounted < FRAMES_COUNT)
	{
		m_Timings[m_FramesCounted] = NestedTimingTree::GetTotalGpuTime();
		m_FramesCounted++;
	}
	if (m_FramesCounted == FRAMES_COUNT)
	{
		std::cout << "FINISHED BENCHMARKING!" << std::endl;
		DumpCSV();
		m_FramesCounted++;
	}
}

void Benchmark::SelectConfig(int idx)
{
	if (idx >= m_Configs.size() || idx < 0)
		return;

	m_ConfigIdx = idx;

	m_FramesCounted = 0;

	for (Callback& callback : m_Callbacks)
	{
		callback(m_Configs[m_ConfigIdx]);
	}
}

void Benchmark::NextConfig()
{
	m_ConfigIdx++;
	SelectConfig(m_ConfigIdx);
}

void Benchmark::RegisterCallback(Callback callback)
{
	m_Callbacks.push_back(callback);
}

void Benchmark::OutputDebug()
{
	for (Debug& debug : m_Debugs)
	{
		debug();
	}
}

void Benchmark::RegisterDebug(Debug debug)
{
	m_Debugs.push_back(debug);
}

void Benchmark::Load(const std::string& filepath)
{
	using namespace nlohmann;

	std::string contents;
	std::ifstream file(filepath);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file \""s + filepath + "\"");

	contents.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	json config = json::parse(contents);

	config["sceneFile"].get_to(m_SceneFile);
	config["configs"].get_to(m_Configs);

	file.close();
}

void Benchmark::DumpCSV()
{
	std::string filename = "Benchmarks/timings/benchmark";

	time_t t = time(nullptr);
	std::tm now{};
	localtime_s(&now, &t);

	filename += "_" + std::to_string(now.tm_year + 1900);
	filename += "-" + std::to_string(now.tm_mon + 1);
	filename += "-" + std::to_string(now.tm_mday);
	filename += "_" + std::to_string(now.tm_hour);
	filename += "-" + std::to_string(now.tm_min);
	filename += "-" + std::to_string(now.tm_sec);
	filename += ".csv";

	std::ofstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file \""s + filename + "\"");

	std::stringstream ss;
	ss << "Frame,Time (ms)" << std::endl;
	for (uint32_t i = 0; i < m_FramesCounted; i++)
	{
		ss << i + 1 << "," << m_Timings[i] << std::endl;
	}

	file << ss.str();

	file.close();
}
