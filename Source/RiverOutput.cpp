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

#include "RiverOutput.h"
#include "RiverOutputEditor.h"
#include "nlohmann/json.hpp"
#include <memory>
#include <unordered_map>

using json = nlohmann::json;

RiverOutput::RiverOutput()
        : GenericProcessor("River Output"),
          spike_schema_({river::FieldDefinition("channel_index", river::FieldDefinition::INT32, 4),
                         river::FieldDefinition("unit_index", river::FieldDefinition::INT32, 4),
                         river::FieldDefinition("data_index", river::FieldDefinition::INT64, 8),
                         river::FieldDefinition("pc_score_0", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_1", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_2", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_3", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_4", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_5", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_6", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_7", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_8", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_9", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_10", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_11", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_12", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_13", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("pc_score_14", river::FieldDefinition::FLOAT, 4),
                         river::FieldDefinition("token", river::FieldDefinition::FIXED_WIDTH_BYTES, 8),
              }) {

    // Start with some sane defaults.
    redis_connection_hostname_ = "127.0.0.1";
    redis_connection_port_ = 6379;

    //stream_name_ = new Parameter("stream_name", juce::String(""), 0, true);
    //stream_name_->addListener(this);
    //parameters.add(stream_name_);

    //channel_mapping_json_ = new Parameter("channel_mapping_json", juce::String(""), 0, true);
    //channel_mapping_json_->addListener(this);
    //parameters.add(channel_mapping_json_);

    // Give some defaults
    writer_max_latency_ms_ = 5;
    writer_max_batch_size_ = 4096;
}


AudioProcessorEditor *RiverOutput::createEditor() {
    editor = std::make_unique<RiverOutputEditor>(this);
    return editor.get();
}


/*void RiverOutput::handleSpike(const SpikeChannel* spikeInfo, const MidiMessage& event, int) {
    SpikeEventPtr spike = SpikeEvent::deserializeFromMessage(event, spikeInfo);
    if (!spike) {
        return;
    }

    jassert(spike->getMetadataValueCount() == 2);
    juce::String spike_token;
    spike->getMetaDataValue(1)->getValue(spike_token);
    
    RiverSpike river_spike;
    int channel_index = getSpikeChannelIndex(spike);
    if (channel_mapping_.find(channel_index) != channel_mapping_.end()) {
        channel_index = channel_mapping_[channel_index];
    }
    river_spike.channel_index = channel_index;
    river_spike.data_index = spike->getTimestamp();
    river_spike.unit_index = ((int) spike->getSortedID()) - 1;

    // Yeah.... hacky.
    river_spike.pc_score_0 = spike->getPcProjPointer()[0];
    river_spike.pc_score_1 = spike->getPcProjPointer()[1];
    river_spike.pc_score_2 = spike->getPcProjPointer()[2];
    river_spike.pc_score_3 = spike->getPcProjPointer()[3];
    river_spike.pc_score_4 = spike->getPcProjPointer()[4];
    river_spike.pc_score_5 = spike->getPcProjPointer()[5];
    river_spike.pc_score_6 = spike->getPcProjPointer()[6];
    river_spike.pc_score_7 = spike->getPcProjPointer()[7];
    river_spike.pc_score_8 = spike->getPcProjPointer()[8];
    river_spike.pc_score_9 = spike->getPcProjPointer()[9];
    river_spike.pc_score_10 = spike->getPcProjPointer()[10];
    river_spike.pc_score_11 = spike->getPcProjPointer()[11];
    river_spike.pc_score_12 = spike->getPcProjPointer()[12];
    river_spike.pc_score_13 = spike->getPcProjPointer()[13];
    river_spike.pc_score_14 = spike->getPcProjPointer()[14];

    jassert(spike_token.length() == 8);
    memcpy(river_spike.token, spike_token.getCharPointer(), 8);

    if (writing_thread_) {
        writing_thread_->enqueue(reinterpret_cast<const char *>(&river_spike), 1);
    } else {
        writer_->WriteBytes(reinterpret_cast<const char *>(&river_spike), 1);
    }
}*/

/*void RiverOutput::handleEvent(const EventChannel* eventInfo, const MidiMessage& msg, int) {
    BinaryEventPtr event = BinaryEvent::deserializeFromMessage(msg, eventInfo);
    if (!event) {
        return;
    }

    const char *ptr = (const char *) event->getBinaryDataPointer();
    size_t data_size = eventInfo->getDataSize();

    // Assert (when compiled in debug) that the sizes match up.
    jassert(((int) data_size == writer_->schema().sample_size()));

    // Assume that the binary data in the event matches the sample size exactly. If it doesn't, crashes will happen!
    if (writing_thread_) {
        writing_thread_->enqueue(ptr, 1);
    } else {
        writer_->WriteBytes(ptr, 1);
    }
}

void RiverOutput::parameterValueChanged(Value &valueThatWasChanged, const String &parameterName) {
    if (parameterName == "stream_name") {
        if (editor) {
            // We already use the Parameter as source of truth for the rest so just ping the editor
            const MessageManagerLock mm;
            ((RiverOutputEditor *) editor.get())->refreshLabelsFromProcessor();
        }
    } else if (parameterName == "channel_mapping_json") {
        std::string json_str = channel_mapping_json_->getValue(0).toString().toStdString();
        if (!json_str.empty()) {
            try {
                json j = json::parse(json_str);
                std::unordered_map<int, int> new_map;
                for (const auto& kv : j.items()) {
                    int from_ch = std::stoi(kv.key());
                    int to_ch = kv.value();
                    std::pair<int, int> p(from_ch, to_ch);
                    new_map.insert(p);
                }
                channel_mapping_ = new_map;
            } catch (const json::exception& e) {
                std::cout << "Failed to parse: "  << e.what() << std::endl;
                channel_mapping_json_->setValue(valueThatWasChanged.getValue(), 0);
            }
        } else {
            channel_mapping_.clear();
        }
    }
}*/

/*bool RiverOutput::enable() {
    auto sn = streamName();
    if (sn.empty() || redis_connection_hostname_.empty() || redis_connection_port_ <= 0) {
        CoreServices::sendStatusMessage("FAILED TO ENABLE");
        return false;
    }

    jassert(!writer_);

    river::RedisConnection connection(
            redis_connection_hostname_,
            redis_connection_port_,
            redis_connection_password_);

    // TODO: what happens if invalid redis connection?
    writer_ = std::make_shared<river::StreamWriter>(connection);

    std::unordered_map<std::string, std::string> metadata;
    if (shouldConsumeSpikes()) {
        if (getTotalSpikeChannels() == 0) {
            // Can't consume spikes if there are no spike channels.
            CoreServices::sendStatusMessage("FAILED TO ENABLE: NO SPIKES");
            return false;
        }

        // Assume that all spike channels have the same details.
        auto spike_channel = getSpikeChannel(0);
        metadata["prepeak_samples"] = std::to_string(spike_channel->getPrePeakSamples());
        metadata["postpeak_samples"] = std::to_string(spike_channel->getPostPeakSamples());
        metadata["sampling_rate"] = std::to_string(CoreServices::getGlobalSampleRate());
    }

    writer_->Initialize(sn, getSchema(), metadata);

    if (editor) {
        // GenericEditor#enable isn't marked as virtual, so need to *upcast* to VisualizerEditor :(
        ((VisualizerEditor *) (editor.get()))->enable();
    }

    // If latency or batch size are nonpositive, write everything synchronously.
    if (maxLatencyMs() > 0 && maxBatchSize() > 0) {
        writing_thread_ = std::make_unique<RiverWriterThread>(writer_, maxBatchSize(), maxLatencyMs());
        writing_thread_->startThread();
        std::cout << "Writing to River asynchronously with stream name " << sn << std::endl;
    } else {
        std::cout << "Writing to River synchronously with stream name " << sn << std::endl;
    }

    return true;
}*/


/*bool RiverOutput::disable() {
    if (writing_thread_) {
        writing_thread_->stopThread(1000);
        writing_thread_.reset();
    }
    if (writer_) {
        writer_->Stop();
        writer_.reset();
    }
    return true;
}*/


void RiverOutput::process(AudioSampleBuffer &buffer) {
    if (writer_) {
        checkForEvents(shouldConsumeSpikes());
    }
}

std::string RiverOutput::streamName() const {
    return "Default"; // stream_name_->getValue(0).toString().toStdString();
}

int64_t RiverOutput::totalSamplesWritten() const {
    if (writer_) {
        return writer_->total_samples_written();
    } else {
        return 0;
    }
}

const std::string &RiverOutput::redisConnectionHostname() const {
    return redis_connection_hostname_;
}

void RiverOutput::setRedisConnectionHostname(const std::string &redisConnectionHostname) {
    redis_connection_hostname_ = redisConnectionHostname;
}

int RiverOutput::redisConnectionPort() const {
    return redis_connection_port_;
}

void RiverOutput::setRedisConnectionPort(int redisConnectionPort) {
    redis_connection_port_ = redisConnectionPort;
}

const std::string &RiverOutput::redisConnectionPassword() const {
    return redis_connection_password_;
}

void RiverOutput::setRedisConnectionPassword(const std::string &redisConnectionPassword) {
    redis_connection_password_ = redisConnectionPassword;
}

void RiverOutput::saveCustomParametersToXml(XmlElement *parentElement) {
    XmlElement *mainNode = parentElement->createNewChildElement("RiverOutput");
    mainNode->setAttribute("hostname", redisConnectionHostname());
    mainNode->setAttribute("port", redisConnectionPort());
    mainNode->setAttribute("password", redisConnectionPassword());
    mainNode->setAttribute("max_latency_ms", maxLatencyMs());
    mainNode->setAttribute("max_batch_size", maxBatchSize());

    if (event_schema_) {
        std::string event_schema_json = event_schema_->ToJson();
        mainNode->setAttribute("event_schema_json", event_schema_json);
    }
}

/*void RiverOutput::loadCustomParametersFromXml() {
    if (parametersAsXml == nullptr) {
        return;
    }

    forEachXmlChildElement(*parametersAsXml, mainNode) {
        if (!mainNode->hasTagName("RiverOutput")) {
            continue;
        }

        redis_connection_hostname_ = mainNode->getStringAttribute("hostname", "127.0.0.1").toStdString();
        redis_connection_port_ = mainNode->getIntAttribute("port", 6379);
        redis_connection_password_ = mainNode->getStringAttribute("password", "").toStdString();

        if (mainNode->hasAttribute("max_latency_ms")) {
            writer_max_latency_ms_ = mainNode->getIntAttribute("max_latency_ms");
        }
        if (mainNode->hasAttribute("max_batch_size")) {
            writer_max_batch_size_ = mainNode->getIntAttribute("max_batch_size");
        }
        if (mainNode->hasAttribute("event_schema_json")) {
            String s = mainNode->getStringAttribute("event_schema_json");
            std::string j = s.toStdString();
            try {
                const river::StreamSchema& schema = river::StreamSchema::FromJson(j);
                setEventSchema(schema);
            } catch (const json::exception& e) {
                std::cout << "Invalid schema json: " << j << " | " << e.what() << std::endl;
                clearEventSchema();
            }
        } else {
            clearEventSchema();
        }
    }

    ((RiverOutputEditor *) editor.get())->refreshSchemaFromProcessor();
    ((RiverOutputEditor *) editor.get())->refreshLabelsFromProcessor();
}*/

void RiverOutput::setEventSchema(const river::StreamSchema& eventSchema) {
    auto p = std::make_shared<river::StreamSchema>(eventSchema);
    event_schema_.swap(p);
}

void RiverOutput::clearEventSchema() {
    event_schema_.reset();
}

bool RiverOutput::shouldConsumeSpikes() const {
    return !event_schema_;
}

river::StreamSchema RiverOutput::getSchema() const {
    if (event_schema_) {
        return *event_schema_;
    }
    return spike_schema_;
}

RiverWriterThread::RiverWriterThread(
        const std::shared_ptr<river::StreamWriter> &writer,
        int capacity_samples,
        int batch_period_ms)
        : juce::Thread("RiverWriter") {
    writer_ = writer;
    writing_queue_ = std::make_unique<AbstractFifo>(capacity_samples);
    batch_period_ms_ = batch_period_ms;

    sample_size_ = writer_->schema().sample_size();

    buffer_.resize(capacity_samples * sample_size_);
}

void RiverWriterThread::run() {
    int start1, size1, start2, size2;
    while (!threadShouldExit()) {
        auto start = Time::getMillisecondCounter();
        writing_queue_->prepareToRead(writing_queue_->getNumReady(),
                                      start1,
                                      size1,
                                      start2,
                                      size2);

        if (size1 > 0) {
            writer_->WriteBytes(&buffer_.front() + start1 * sample_size_, size1);
        }

        if (size2 > 0) {
            writer_->WriteBytes(&buffer_.front() + start2 * sample_size_, size2);
        }

        writing_queue_->finishedRead(size1 + size2);

        Time::waitForMillisecondCounter(start + batch_period_ms_);
    }
}

void RiverWriterThread::enqueue(const char *data, int num_samples) {
    int start1, size1, start2, size2;
    writing_queue_->prepareToWrite(num_samples, start1, size1, start2, size2);

    if (size1 > 0) {
        memcpy(&buffer_.front() + start1 * sample_size_, data, size1 * sample_size_);
    }

    if (size2 > 0) {
        memcpy(&buffer_.front() + start2 * sample_size_, data + size1 * sample_size_, size2 * sample_size_);
    }
    jassert(size1 + size2 == num_samples);
    writing_queue_->finishedWrite(size1 + size2);
}
