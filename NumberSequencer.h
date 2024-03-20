#ifndef NUMBER_SEQUENCER
#define NUMBER_SEQUENCER

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <type_traits>
#include <algorithm>

namespace _numberSequencer
{ 

template<size_t size>
class SequenceString
{
public:
    constexpr inline SequenceString(const char (&str)[size])
    {
        std::copy_n(str, size, value);
    }

    char value[size];
};


class NumberSequencer final
{
    template<int size>
    using DisintegratedSequences = std::array<std::array<std::int64_t, size>, size - 1>;

    template<int size>
    class KnownNumbersStruct
    {
    public:
        constexpr inline KnownNumbersStruct()
        {
            number = 0;
            lastNumberMode = true;
            knownNumbers = std::array<std::int64_t, size>();
        }

        std::int64_t number;
        bool lastNumberMode;
        std::array<std::int64_t, size> knownNumbers;
    };

    constexpr static inline int GetDigit(char c)
    {
        return c >= '0' && c <= '9' ? c - '0' : -1;
    }

    template<int size, int resultingSize>
    constexpr static inline bool CheckRules(
        const KnownNumbersStruct<size>& knownNumbersStruct,
        const std::array<std::int64_t, resultingSize>& continuedSequence,
        size_t continuedSequenceCounter,
        size_t counter
    )
    {
        return ((knownNumbersStruct.lastNumberMode ? 
            continuedSequence[continuedSequenceCounter] : counter++) < knownNumbersStruct.number);
    }

    constexpr static int Atoi(const std::string& str)
    {
        int res = 0;
        int tens = 1;

        for (int i = str.size() - 1; i >= 0; --i)
        {
            res += (GetDigit(str[i]) * tens);
            tens *= 10;
        }

        return res;
    }

    constexpr static bool CheckForCorrectFormat(const char* sequenceString)
    {
        std::string acceptableSymbols = "01234567890.() ";
        bool resBool = true;

        for (int i = 0; i < sequenceString[i] != '\0'; ++i)
        {
            for (auto& acceptableSymbol : acceptableSymbols)
            {
                resBool = false;
                if (acceptableSymbol == sequenceString[i])
                {
                    resBool = true;
                    break;
                }
            }
            if (!resBool) break;
        }

        return resBool;
    }

    constexpr static int GetKnownNumberAmount(const char* sequenceString)
    {
        int countNumbers = 0;
        int countDigitAmount = 0;

        for (int i = 0; sequenceString[i] != '\0'; ++i)
        {
            if (GetDigit(sequenceString[i]) != -1)
            {
                ++countDigitAmount;
            }
            else if (countDigitAmount)
            {
                ++countNumbers;
                countDigitAmount = 0;
            }
        }

        return --countNumbers;
    }

    template<SequenceString seqStr>
    constexpr static auto GetKnownNumbers()
    {
        std::string numberBuffer = "";
        bool collectSequenceNumbers = true;

        KnownNumbersStruct<GetKnownNumberAmount(seqStr.value)> knownNumbersStruct;

        int count = 0;

        for (int i = 0; seqStr.value[i] != '\0'; ++i)
        {
            if (knownNumbersStruct.lastNumberMode && seqStr.value[i] == '(')
            {
                knownNumbersStruct.lastNumberMode = false;
            }
            else if (seqStr.value[i] == ')') break;

            if (seqStr.value[i] == '.')
            {
                collectSequenceNumbers = false;
            }
            else if (GetDigit(seqStr.value[i]) != -1)
            {
                numberBuffer += seqStr.value[i];
            }
            else if (numberBuffer.length() > 0)
            {
                knownNumbersStruct.knownNumbers[count++] = Atoi(numberBuffer);
                numberBuffer = "";
            }
        }

        knownNumbersStruct.number = Atoi(numberBuffer);

        return knownNumbersStruct;
    }

    template<int size>
    constexpr static bool CheckForLinearSequence(const std::array<std::int64_t, size>& currentDisintSequence)
    {
        for (int i = 0; i < currentDisintSequence.size(); ++i)
        {
            if (currentDisintSequence[i] < -1 || currentDisintSequence[i] > 0)
            {
                return false;
            }
        }

        return true;
    }

    template<int size>
    constexpr static auto DisintegrateSequence(const std::array<std::int64_t, size>& knownNumbers)
    {
        DisintegratedSequences<size> disintSequences;
        std::array<std::int64_t, size> sequenceToDisint = knownNumbers;
        std::array<std::int64_t, size> currentDisintSequence;
        bool foundPattern = true;

        for (int i = 0; i < disintSequences.size(); ++i)
        {
            disintSequences[i].fill(-1);
        }

        for (int i = 0; i < knownNumbers.size() - 1; ++i)
        {
            currentDisintSequence.fill(-1);
            for (int j = 0; j + i + 1 < sequenceToDisint.size(); ++j)
            {
                currentDisintSequence[j] = sequenceToDisint[j + 1] - sequenceToDisint[j];
            }

            disintSequences[i] = currentDisintSequence;

            foundPattern = CheckForLinearSequence<currentDisintSequence.size()>(currentDisintSequence);
            if (foundPattern) break;

            sequenceToDisint = currentDisintSequence;
        }

        if (!foundPattern)
        {
            disintSequences[0][0] = -1;
        }

        return disintSequences;
    }

    template<int size, int resultingSize>
    constexpr static auto ContinueSequenceLinearly(
        const KnownNumbersStruct<size>& knownNumbersStruct,
        const DisintegratedSequences<size>& disintSequences
    )
    {
        DisintegratedSequences<size> reversedDisintSequences;

        for (int i = 0; i < disintSequences.size(); ++i)
        {
            reversedDisintSequences[i] = disintSequences[disintSequences.size() - i - 1];
        }

        std::array<std::int64_t, resultingSize> continuedSequence;

        for (int i = 0; i < knownNumbersStruct.knownNumbers.size(); ++i)
        {
            continuedSequence[i] = knownNumbersStruct.knownNumbers[i];
        }

        std::array<std::int64_t, size - 1> lastNumbers;

        for (int i = 0; i < lastNumbers.size(); ++i)
        {
            // Use last existing value of every disintegrated sequence
            lastNumbers[i] = reversedDisintSequences[i][i] == -1 ? 0 : reversedDisintSequences[i][i];
        }

        size_t counter = 0;
        size_t continuedSequenceCounter = knownNumbersStruct.knownNumbers.size() - 1;
        while (CheckRules<size, resultingSize>(knownNumbersStruct, continuedSequence, continuedSequenceCounter, counter++))
        {
            for (size_t i = 0; i < disintSequences.size() - 1; ++i)
            {
                lastNumbers[i + 1] += lastNumbers[i];
            }
            continuedSequence[continuedSequenceCounter + 1] = (continuedSequence[continuedSequenceCounter] + lastNumbers.back());

            ++continuedSequenceCounter;
        }

        return continuedSequence;
    }

public:
    template<SequenceString seqStr>
    constexpr static auto SequenceDet()
    {
        constexpr bool isFormatCorrect = CheckForCorrectFormat(seqStr.value);
        static_assert(isFormatCorrect, "Check for correct format failed");

        constexpr auto knownNumbersStruct = GetKnownNumbers<seqStr>();

        constexpr auto disintSequence = DisintegrateSequence<
            knownNumbersStruct.knownNumbers.size()>
            (
                knownNumbersStruct.knownNumbers
            );
        static_assert(disintSequence[0][0] != -1, "Cannot disintegrate sequence\n");

        constexpr auto continuedSequence = ContinueSequenceLinearly<
            knownNumbersStruct.knownNumbers.size(),
            knownNumbersStruct.knownNumbers.size() + knownNumbersStruct.number>
            (
                knownNumbersStruct, disintSequence
            );

        return continuedSequence;
    }

    ~NumberSequencer() = delete;

};

};

template<_numberSequencer::SequenceString seqStr>
constexpr inline auto operator""_nseq()
{
    return _numberSequencer::NumberSequencer::SequenceDet<seqStr>();
}

#endif