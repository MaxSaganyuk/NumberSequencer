#include "NumberSequencer.h"

bool NumberSequencer::CheckForCorrectFormat(const std::string& sequenceString) {
	std::string acceptableSymbols = "0123456789.(), ";
	bool resBool = true;

	for (auto& c : sequenceString) {
		for (auto& acceptableSymbol : acceptableSymbols) {
			resBool = false;
			if (c == acceptableSymbol) {
				resBool = true;
				break;
			}
		}
		if (!resBool) break;
	}

	return resBool;
}

std::vector<llint> NumberSequencer::GetKnownNumbers(const std::string& sequenceString, llint& number, bool& lastNumberMode) {
	std::string numberBuffer = "";
	std::vector<llint> knownNumbers;
	bool collectSequenceNumbers = true;

	for (auto iter = sequenceString.cbegin(); iter != sequenceString.cend(); ++iter) {
		if (lastNumberMode && *iter == '(') lastNumberMode = false;
		else if (*iter == ')') break;

		if (*iter == '.') collectSequenceNumbers = false;
		else if (isdigit(*iter)) numberBuffer += *iter;
		else if (numberBuffer.length() > 0) {
			knownNumbers.push_back(atoi(numberBuffer.c_str()));
			numberBuffer = "";
		}
	}

	number = atoi(numberBuffer.c_str());

	return knownNumbers;
}

void NumberSequencer::CheckForLinearSequence(const std::vector<llint>& currentDisintSequence, bool& foundPattern)
{
	for (auto iter = currentDisintSequence.cbegin(); iter != currentDisintSequence.cend(); ++iter) {
		if (*iter != 0) {
			foundPattern = false;
			break;
		}
	}
}

void NumberSequencer::CheckForRecursiveSequence(
	const std::vector<llint>& sequenceToDisint, 
	const std::vector<std::vector<llint>>& disintSequences, 
	bool& foundPattern)
{
	foundPattern = true;

	for (auto iter = disintSequences.cbegin(); iter != disintSequences.cend(); ++iter) {
		for (auto interIter = iter->cbegin(), knownIter = sequenceToDisint.cbegin(); interIter != iter->cend(); ++interIter, ++knownIter) {
			if (*interIter != *knownIter) {
				foundPattern = false;
				break;
			}
		}
	}
}


bool NumberSequencer::DisintegrateSequence(
	const std::vector<llint>& knownNumbers, 
	std::vector<std::vector<llint>>& disintSequences,
	SequenceType& seqType) {
	
	std::vector<llint> sequenceToDisint = knownNumbers;
	std::vector<llint> currentDisintSequence;
	bool foundPattern = true;
	seqType = SequenceType::LINEAR;

	for (int i = 0; i < knownNumbers.size() - 1; ++i) {
		currentDisintSequence.clear();
		for (auto iter = sequenceToDisint.cbegin(); iter + 1 != sequenceToDisint.cend(); ++iter) {
			currentDisintSequence.push_back(*(iter + 1) - *(iter));
		}

		CheckForLinearSequence(currentDisintSequence, foundPattern);
		if (foundPattern) break;

		disintSequences.push_back(currentDisintSequence);
		sequenceToDisint = currentDisintSequence;
	}

	if (!foundPattern) {
		CheckForRecursiveSequence(knownNumbers, disintSequences, foundPattern);
		seqType = SequenceType::RECURSIVE;
	}

	return foundPattern;
}

std::vector<llint> NumberSequencer::ContinueSequenceLinearly(
	std::vector<llint>& knownNumbers,
	std::vector<std::vector<llint>>& disintSequences,
	llint number,
	bool lastNumberMode
){
	std::vector<llint> continuedSequence = knownNumbers;

	std::reverse(disintSequences.begin(), disintSequences.end());

	size_t counter = 0;
	while (CheckRules(continuedSequence, number, lastNumberMode, counter++)) {
		for (size_t i = 0; i < disintSequences.size() - 1; ++i) {
			disintSequences[i + 1].push_back(disintSequences[i + 1].back() + disintSequences[i].back());
		}
		continuedSequence.push_back(continuedSequence.back() + disintSequences.back().back());
	}

	if (lastNumberMode && continuedSequence.back() > number) return {};

	return continuedSequence;
}

std::vector<llint> NumberSequencer::SequenceDet(const std::string& sequenceString) {
	if (CheckForCorrectFormat(sequenceString)) {
		llint number = 0;
		bool lastNumberMode = true;
		SequenceType seqType;

		std::vector<llint> knownNumbers = GetKnownNumbers(sequenceString, number, lastNumberMode);

		std::vector<std::vector<llint>> disintSequences;
		if (DisintegrateSequence(knownNumbers, disintSequences, seqType)) {
			std::vector<llint> continuedSequence;

			switch (seqType){
			case SequenceType::LINEAR:
				continuedSequence = ContinueSequenceLinearly(knownNumbers, disintSequences, number, lastNumberMode);
			}

			return continuedSequence;
		}
	}

	return {};
}


std::vector<llint> operator""_nseq(const char* sequenceString, std::size_t) {
	return NumberSequencer::SequenceDet(std::string(sequenceString));
}