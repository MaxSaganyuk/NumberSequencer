#pragma once

#include <string>
#include <vector>

using llint = long long;

class NumberSequencer final {
private:
	enum class SequenceType {
		LINEAR,
		RECURSIVE
	};

	static bool CheckForCorrectFormat(const std::string& sequenceString);
	static std::vector<llint> GetKnownNumbers(const std::string& sequenceString, llint& number, bool& lastNumberMode);

	static void CheckForLinearSequence(const std::vector<llint>& currentDisintSequence, bool& foundPattern);
	static void CheckForRecursiveSequence(
		const std::vector<llint>& sequenceToDisint,
		const std::vector<std::vector<llint>>& disintSequences,
		bool& foundPattern);
	static bool DisintegrateSequence(
		const std::vector<llint>& knownNumbers, 
		std::vector<std::vector<llint>>& disintSequences,
		SequenceType& seqType
	);

	static std::vector<llint> ContinueSequenceLinearly(
		std::vector<llint>& knownNumbers,
		std::vector<std::vector<llint>>& disintSequences,
		llint number,
		bool lastNumberMode
	);
	static void ContinueSequenceRecursivly(std::vector<llint>& knownNumbers);

	static inline bool CheckRules(const std::vector<llint>& continuedSequence, llint number, bool lastNumberMode, size_t counter) {
		return (lastNumberMode ? continuedSequence.back() : counter) < number;
	}

public:
	static __declspec(dllexport) std::vector<llint> SequenceDet(const std::string& sequenceString);
	~NumberSequencer() = delete;
};

__declspec(dllexport) std::vector<llint> operator""_nseq(const char* sequenceString, std::size_t);