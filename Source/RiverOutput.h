/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __RIVEROUTPUT_H_F7BDA585__
#define __RIVEROUTPUT_H_F7BDA585__

#include <ProcessorHeaders.h>
#include "river/river.h"

class RiverWriterThread : public juce::Thread {
public:
    RiverWriterThread(const std::shared_ptr<river::StreamWriter> &writer,
                      int capacity_samples,
                      int batch_period_ms);
    ~RiverWriterThread() override = default;

    void run() override;

    void enqueue(const char *data, int num_samples);
private:
    std::unique_ptr<AbstractFifo> writing_queue_;
    std::vector<char> buffer_;
    int batch_period_ms_;
    int sample_size_;
    std::shared_ptr<river::StreamWriter> writer_;
};

/**
 *  TODO
    @see GenericProcessor
 */
class RiverOutput : public GenericProcessor
{
public:
    RiverOutput();

    ~RiverOutput() override = default;

    /** Searches for events and triggers the River output when appropriate. */
    void process(AudioSampleBuffer &buffer) override;

    //void handleSpike(const SpikeChannel *spikeInfo, const MidiMessage &event, int samplePosition) override;
   // void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

    /** Called immediately prior to the start of data acquisition. */
    //bool enable() override;

    /** Called immediately after the end of data acquisition. */
    //bool disable() override;

    /** Creates the RiverOutputEditor. */
    AudioProcessorEditor *createEditor() override;

    /** To/from conversion XML */
    void saveCustomParametersToXml (XmlElement* parentElement) override;
    /** Load custom settings from XML*/
    //void loadCustomParametersFromXml() override;

    // Parameter::Listener
    //void parameterValueChanged(Value &valueThatWasChanged, const String &parameterName) override;

    //
    // Non-override methods:
    //
    const std::string &redisConnectionHostname() const;
    void setRedisConnectionHostname(const std::string &redisConnectionHostname);
    int redisConnectionPort() const;
    void setRedisConnectionPort(int redisConnectionPort);
    const std::string &redisConnectionPassword() const;
    void setRedisConnectionPassword(const std::string &redisConnectionPassword);

    void setEventSchema(const river::StreamSchema& eventSchema);
    void clearEventSchema();
    bool shouldConsumeSpikes() const;

    river::StreamSchema getSchema() const;

    std::string streamName() const;
    int64_t totalSamplesWritten() const;

    int maxBatchSize() const {
        return writer_max_batch_size_;
    }

    int maxLatencyMs() const {
        return writer_max_latency_ms_;
    }

    void setMaxBatchSize(int maxBatchSize) {
        writer_max_batch_size_ = maxBatchSize;
    }

    void setMaxLatencyMs(int maxLatencyMs) {
        writer_max_latency_ms_ = maxLatencyMs;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RiverOutput)

    // For writing spikes to River. Ensure it's packed so that padding doesn't mess up the size of the struct.
    typedef struct RiverSpike {
        int32_t channel_index;
        int32_t unit_index;
        int64_t data_index;
        // Hackyyyyy
        float pc_score_0;
        float pc_score_1;
        float pc_score_2;
        float pc_score_3;
        float pc_score_4;
        float pc_score_5;
        float pc_score_6;
        float pc_score_7;
        float pc_score_8;
        float pc_score_9;
        float pc_score_10;
        float pc_score_11;
        float pc_score_12;
        float pc_score_13;
        float pc_score_14;

        // Set to all zeros if not present.
        char token[8];
    }; // __attribute__((__packed__)) RiverSpike;
    const river::StreamSchema spike_schema_;

    // If this is set, then we should listen to events, not spikes.
    std::shared_ptr<river::StreamSchema> event_schema_;

    std::shared_ptr<river::StreamWriter> writer_;
    std::unique_ptr<RiverWriterThread> writing_thread_;

    // Access this via method instead of raw
    Parameter *stream_name_;
    Parameter *channel_mapping_json_;
    std::unordered_map<int, int> channel_mapping_;

    std::string redis_connection_hostname_;
    int redis_connection_port_;
    std::string redis_connection_password_;

    int writer_max_batch_size_;
    int writer_max_latency_ms_;
};


#endif  // __RIVEROUTPUT_H_F7BDA585__
