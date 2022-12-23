/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

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

#include "RiverOutputEditor.h"
#include "SchemaListBox.h"

RiverOutputEditor::RiverOutputEditor(GenericProcessor *parentNode)
        : VisualizerEditor(parentNode, "River Output", 350) {

    hostnameLabel = newStaticLabel("Hostname", 10, 25, 80, 20);
    hostnameLabelValue = newInputLabel("hostnameLabelValue", "Set the hostname for River", 15, 42, 80, 18);
    hostnameLabelValue->addListener(this);

    portLabel = newStaticLabel("Port", 10, 65, 80, 20);
    portLabelValue = newInputLabel("hostnamePortValue", "Set the port for River", 15, 82, 60, 18);
    portLabelValue->addListener(this);

    passwordLabel = newStaticLabel("Password", 105, 25, 100, 20);
    passwordLabelValue = newInputLabel("hostnamePasswordValue", "Set the password for River", 110, 42, 100, 18);
    passwordLabelValue->addListener(this);

    optionsPanel = new Component("River Options Panel");
    // initial bounds, to be expanded
    const int C_TEXT_HT = 25;
    const int LEFT_EDGE = 30;
    const int TOP_EDGE = 15;
    const int TAB_WIDTH = 25;
    const int LABEL_VALUE_GAP = 20;
    int xPos, yPos;

    xPos = LEFT_EDGE;
    yPos = TOP_EDGE;

    optionsPanelTitle = new Label("RiverOptionsTitle", "River Options");
    optionsPanelTitle->setBounds(xPos, yPos, 400, 50);
    optionsPanelTitle->setFont(Font(20, Font::bold));
    optionsPanel->addAndMakeVisible(optionsPanelTitle);

    Font subtitleFont(16, Font::bold);

    /* ~~~~~~~~ Input type ~~~~~~~~ */

    xPos = LEFT_EDGE;
    yPos += 45;

    inputTypeTitle = new Label("InputType", "Input Type");
    inputTypeTitle->setBounds(xPos, yPos, 200, 50);
    inputTypeTitle->setFont(subtitleFont);
    optionsPanel->addAndMakeVisible(inputTypeTitle);

    /* -------- Radio group #1: Spikes --------- */

    xPos += TAB_WIDTH;
    yPos += 45;

    inputTypeSpikeButton = new ToggleButton("Spikes");
    inputTypeSpikeButton->setRadioGroupId(inputTypeRadioId, dontSendNotification);
    inputTypeSpikeButton->setBounds(xPos, yPos, 150, C_TEXT_HT);
    inputTypeSpikeButton->setToggleState(true, dontSendNotification);
    inputTypeSpikeButton->setTooltip("Emit spike events to River with a preset schema");
    inputTypeSpikeButton->addListener(this);
    optionsPanel->addAndMakeVisible(inputTypeSpikeButton);

    /* -------- Radio group #1: Events (with a custom Schema) --------- */

    yPos += 40;

    inputTypeEventButton = new ToggleButton("Events");
    inputTypeEventButton->setRadioGroupId(inputTypeRadioId, dontSendNotification);
    inputTypeEventButton->setBounds(xPos, yPos, 340, C_TEXT_HT);
    inputTypeEventButton->setToggleState(false, dontSendNotification);
    inputTypeEventButton->setTooltip("Emit custom events to River with a given schema");
    inputTypeEventButton->addListener(this);
    optionsPanel->addAndMakeVisible(inputTypeEventButton);
    yPos += 60;

    xPos = LEFT_EDGE;
    fieldNameLabel = newStaticLabel("Field Name:", xPos, yPos, 140, C_TEXT_HT, optionsPanel);
    fieldNameLabelValue = newInputLabel("fieldNameLabelValue",
                                        "Name of the new field",
                                        xPos,
                                        yPos + LABEL_VALUE_GAP,
                                        100,
                                        C_TEXT_HT,
                                        optionsPanel);

    xPos += fieldNameLabel->getBounds().getWidth() + 4;
    fieldTypeLabel = newStaticLabel("Field Type:", xPos, yPos, 80, C_TEXT_HT, optionsPanel);
    optionsPanel->addAndMakeVisible(fieldTypeLabel);
    fieldTypeComboBox = new ComboBox("Field Type:");
    fieldTypeComboBox->setBounds(xPos, yPos + LABEL_VALUE_GAP, 100, C_TEXT_HT);
    fieldTypeComboBox->addItem("DOUBLE", 1);
    fieldTypeComboBox->addItem("FLOAT", 2);
    fieldTypeComboBox->addItem("INT32", 3);
    fieldTypeComboBox->addItem("INT64", 4);
    fieldTypeComboBox->addListener(this);
    optionsPanel->addAndMakeVisible(fieldTypeComboBox);

    xPos += fieldTypeComboBox->getBounds().getWidth() + 20;

    addFieldButton = new UtilityButton("+", titleFont);
    addFieldButton->setBounds(xPos, yPos + LABEL_VALUE_GAP, 20, C_TEXT_HT);
    addFieldButton->setRadius(3.0f);
    addFieldButton->addListener(this);
    optionsPanel->addAndMakeVisible(addFieldButton);

    xPos += addFieldButton->getBounds().getWidth() + 4;
    removeSelectedFieldButton = new UtilityButton("-", titleFont);
    removeSelectedFieldButton->setBounds(xPos, yPos + LABEL_VALUE_GAP, 20, C_TEXT_HT);
    removeSelectedFieldButton->setRadius(3.0f);
    removeSelectedFieldButton->addListener(this);
    optionsPanel->addAndMakeVisible(removeSelectedFieldButton);

    xPos = LEFT_EDGE;
    yPos += 60;

    asyncLatencyMsLabel = newStaticLabel("Max Latency (ms)", xPos, yPos, 140, C_TEXT_HT, optionsPanel);
    asyncLatencyMsLabelValue = newInputLabel("asyncLatencyMsLabelValue",
                                             "Maximum latency in milliseconds allowed between receiving a sample and writing it to River. "
                                             "Set to 0 or negative to send synchronously.",
                                             xPos,
                                             yPos + LABEL_VALUE_GAP,
                                             100,
                                             C_TEXT_HT,
                                             optionsPanel);
    asyncLatencyMsLabelValue->addListener(this);

    xPos += asyncLatencyMsLabel->getBounds().getWidth() + 4;
    asyncBatchSizeLabel = newStaticLabel("Max Batch Size", xPos, yPos, 140, C_TEXT_HT, optionsPanel);
    asyncBatchSizeLabelValue = newInputLabel("asyncBatchSizeLabelValue",
                                             "Maximum number of samples to write in a batch to River. "
                                             "Set to 0 or negative to send synchronously.",
                                             xPos,
                                             yPos + LABEL_VALUE_GAP,
                                             100,
                                             C_TEXT_HT,
                                             optionsPanel);
    asyncBatchSizeLabelValue->addListener(this);

    xPos = LEFT_EDGE;
    yPos += 60;
    schemaList = new SchemaListBox();
    schemaList->setBounds(xPos, yPos, 300, 400);
    optionsPanel->addAndMakeVisible(schemaList);


    // Move to right half, even with the title
    xPos = LEFT_EDGE + 400;
    yPos = inputTypeTitle->getY();
    streamNameLabel = newStaticLabel("Stream Name", xPos, yPos, 80, 20, optionsPanel);
    streamNameLabelValue = newStaticLabel("N/A",
                                          xPos,
                                          yPos + LABEL_VALUE_GAP,
                                          200,
                                          18,
                                          optionsPanel);

    // Move to right half, in line with the title
    yPos += 60;
    totalSamplesWrittenLabel = newStaticLabel("Samples Written", xPos, yPos, 150, 20, optionsPanel);
    totalSamplesWrittenLabelValue = newStaticLabel("0",
                                                   xPos,
                                                   yPos + LABEL_VALUE_GAP,
                                                   120,
                                                   18,
                                                   optionsPanel);


    // Update the bounds of the options panel to fit all of the components in it:
    juce::Rectangle<int> opBounds(0, 0, 1, 1);
    for (const Component *component : {
            dynamic_cast<Component *>(optionsPanel.get()),
            dynamic_cast<Component *>(optionsPanelTitle.get()),
            dynamic_cast<Component *>(inputTypeTitle.get()),
            dynamic_cast<Component *>(inputTypeSpikeButton.get()),
            dynamic_cast<Component *>(inputTypeEventButton.get()),
            dynamic_cast<Component *>(fieldNameLabel.get()),
            dynamic_cast<Component *>(fieldNameLabelValue.get()),
            dynamic_cast<Component *>(fieldTypeLabel.get()),
            dynamic_cast<Component *>(fieldTypeComboBox.get()),
            dynamic_cast<Component *>(addFieldButton.get()),
            dynamic_cast<Component *>(removeSelectedFieldButton.get()),
            dynamic_cast<Component *>(schemaList.get()),
            dynamic_cast<Component *>(streamNameLabel.get()),
            dynamic_cast<Component *>(streamNameLabelValue.get()),
            dynamic_cast<Component *>(totalSamplesWrittenLabel.get()),
            dynamic_cast<Component *>(totalSamplesWrittenLabelValue.get()),
            dynamic_cast<Component *>(asyncBatchSizeLabel.get()),
            dynamic_cast<Component *>(asyncBatchSizeLabelValue.get()),
            dynamic_cast<Component *>(asyncLatencyMsLabel.get()),
            dynamic_cast<Component *>(asyncLatencyMsLabelValue.get()),
    }) {
        opBounds = opBounds.getUnion(component->getBounds());
    }
    // Plus a little buffer:
    opBounds.setBottom(opBounds.getBottom() + 10);
    opBounds.setRight(opBounds.getRight() + 10);
    optionsPanel->setBounds(opBounds);

    refreshLabelsFromProcessor();
}

Visualizer* RiverOutputEditor::createNewCanvas() {
    canvas = new RiverOutputCanvas(getProcessor());
    return canvas;
}

void RiverOutputEditor::comboBoxChanged(ComboBox *) {}

void RiverOutputEditor::buttonClicked(Button *button) {
    if (isPlaying) {
        CoreServices::sendStatusMessage("Cannot update schema while running.");
        return;
    }

    if (button == addFieldButton) {
        const String &fieldName = fieldNameLabelValue->getText();
        if (fieldName.isEmpty() || fieldTypeComboBox->getSelectedId() <= 0) {
            return;
        }

        river::FieldDefinition::Type type;
        int size;
        switch (fieldTypeComboBox->getSelectedId()) {
            case 1:
                type = river::FieldDefinition::DOUBLE;
                size = 8;
                break;
            case 2:
                type = river::FieldDefinition::FLOAT;
                size = 4;
                break;
            case 3:
                type = river::FieldDefinition::INT32;
                size = 4;
                break;
            case 4:
                type = river::FieldDefinition::INT64;
                size = 8;
                break;
            default:
                return;
        }
        schemaList->addItem(river::FieldDefinition(fieldName.toStdString(), type, size));
    } else if (button == removeSelectedFieldButton) {
        schemaList->removeSelectedRow();
    }
    updateProcessorSchema();
}

void RiverOutputEditor::updateProcessorSchema() {
    auto processor = dynamic_cast<RiverOutput *>(getProcessor());

    if (inputTypeSpikeButton->getToggleState()) {
        // Clearing event schema forces use of the spike schema / spike input type.
        processor->clearEventSchema();
    } else if (inputTypeEventButton->getToggleState()) {
        auto field_definitions = schemaList->fieldDefinitions();
        if (!field_definitions.empty()) {
            processor->setEventSchema(river::StreamSchema(field_definitions));
        }
    } else {
        // Can happen transiently where both are off briefly
    }
}


void RiverOutputEditor::labelTextChanged(Label *label) {
    auto river = (RiverOutput *) getProcessor();
    if (label == hostnameLabelValue) {
        river->setRedisConnectionHostname(label->getText().toStdString());
    } else if (label == portLabelValue) {
        int port = label->getText().getIntValue();
        if (port > 0) {
            river->setRedisConnectionPort(port);
            lastPortValue = label->getText().toStdString();
        } else {
            label->setText(juce::String(lastPortValue), dontSendNotification);
        }
    } else if (label == passwordLabelValue) {
        river->setRedisConnectionPassword(label->getText().toStdString());
    } else if (label == fieldNameLabelValue) {
        // Nothing to do.
    } else if (label == asyncLatencyMsLabelValue) {
        river->setMaxLatencyMs(label->getText().getIntValue());
    } else if (label == asyncBatchSizeLabelValue) {
        river->setMaxBatchSize(label->getText().getIntValue());
    }
}

void RiverOutputEditor::refreshLabelsFromProcessor() {
    auto river = (RiverOutput *) getProcessor();
    lastPortValue = std::to_string(river->redisConnectionPort());

    hostnameLabelValue->setText(river->redisConnectionHostname(), dontSendNotification);
    portLabelValue->setText(lastPortValue, dontSendNotification);
    passwordLabelValue->setText(river->redisConnectionPassword(), dontSendNotification);
    streamNameLabelValue->setText(river->streamName(), dontSendNotification);

    totalSamplesWrittenLabelValue->setText(juce::String(river->totalSamplesWritten()), dontSendNotification);

    asyncLatencyMsLabelValue->setText(juce::String(river->maxLatencyMs()), dontSendNotification);
    asyncBatchSizeLabelValue->setText(juce::String(river->maxBatchSize()), dontSendNotification);
}

void RiverOutputEditor::refreshSchemaFromProcessor() {
    auto processor = dynamic_cast<RiverOutput *>(getProcessor());
    if (processor->shouldConsumeSpikes()) {
        inputTypeEventButton->setToggleState(false, dontSendNotification);
        inputTypeSpikeButton->setToggleState(true, dontSendNotification);
    } else {
        inputTypeSpikeButton->setToggleState(false, dontSendNotification);
        inputTypeEventButton->setToggleState(true, dontSendNotification);
        schemaList->clearItems();
        for (const auto& field : processor->getSchema().field_definitions) {
            schemaList->addItem(field);
        }
    }
}

RiverOutputCanvas::RiverOutputCanvas(GenericProcessor *n) : processor(n) {
    editor = dynamic_cast<RiverOutputEditor *>(processor->editor.get());
    viewport = new Viewport();
    Component *optionsPanel = editor->getOptionsPanel();
    viewport->setViewedComponent(optionsPanel, false);
    viewport->setScrollBarsShown(true, true);
    addAndMakeVisible(viewport);

    // Just updating the # of samples written, so can update pretty slow
    refreshRate = 4;
}

void RiverOutputCanvas::refreshState() {}
void RiverOutputCanvas::update() {}

void RiverOutputCanvas::refresh() {
    editor->refreshLabelsFromProcessor();
    editor->repaint();
}

void RiverOutputCanvas::beginAnimation() {
    startCallbacks();
}
void RiverOutputCanvas::endAnimation() {
    stopCallbacks();
}

//void RiverOutputCanvas::setParameter(int, float) {}
//void RiverOutputCanvas::setParameter(int, int, int, float) {}

void RiverOutputCanvas::paint(Graphics &g) {
    ColourGradient editorBg = editor->getBackgroundGradient();
    g.fillAll(editorBg.getColourAtPosition(0.5)); // roughly matches editor background (without gradient)
}

void RiverOutputCanvas::resized() {
    viewport->setBounds(0, 0, getWidth(), getHeight());
}

