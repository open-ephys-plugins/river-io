//
// Created by Paul Botros on 9/18/20.
//

#ifndef OPEN_EPHYS_GUI_SCHEMALISTBOX_H
#define OPEN_EPHYS_GUI_SCHEMALISTBOX_H

#include <VisualizerEditorHeaders.h>
#include <VisualizerWindowHeaders.h>
#include <EditorHeaders.h>
#include "river/river.h"
#include <sstream>

class SchemaListBox : public Component, public ListBoxModel {
public:
    SchemaListBox() {
        box.setModel(this);
        box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
        box.setRowHeight(30);
        addAndMakeVisible(box);
    }

    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height,
                          bool rowIsSelected) override {
        if (!isPositiveAndBelow(rowNumber, (int) fieldDefinitions_.size())) {
            return;
        }
        g.fillAll(Colour::greyLevel(rowIsSelected ? 0.15f : 0.05f));
        g.setFont(18);
        g.setColour(Colours::whitesmoke);

        auto field_def = fieldDefinitions_[rowNumber];

        std::stringstream ss;
        ss << field_def.name << " [";
        switch (field_def.type) {
            case river::FieldDefinition::DOUBLE:
                ss << "DOUBLE";
                break;
            case river::FieldDefinition::FLOAT:
                ss << "FLOAT";
                break;
            case river::FieldDefinition::INT32:
                ss << "INT32";
                break;
            case river::FieldDefinition::INT64:
                ss << "INT64";
                break;
            case river::FieldDefinition::FIXED_WIDTH_BYTES:
            case river::FieldDefinition::VARIABLE_WIDTH_BYTES:
            default:
                ss << "UNKNOWN";
                break;
        }
        ss << "]";

        g.drawFittedText(String(ss.str()), {6, 0, width - 12, height}, Justification::centredLeft, 1, 1.f);
    }

    int getNumRows() override {
        return fieldDefinitions_.size();
    }

    void removeSelectedRow() {
        if (box.getSelectedRow(0) >= 0) {
            fieldDefinitions_.erase(fieldDefinitions_.begin() + box.getSelectedRow(0));
            box.updateContent();
        }
    }

    void clearItems() {
        fieldDefinitions_.clear();
        box.updateContent();
    }

    void addItem(const river::FieldDefinition &fieldDefinition) {
        fieldDefinitions_.push_back(fieldDefinition);
        box.updateContent();
        box.selectRow(fieldDefinitions_.size() - 1);
    }

    void resized() override {
        box.setBounds(getLocalBounds());
    }

    const std::vector<river::FieldDefinition> &fieldDefinitions() {
        return fieldDefinitions_;
    }

private:
    ListBox box;
    std::vector<river::FieldDefinition> fieldDefinitions_;
};

#include <EditorHeaders.h>
#include <VisualizerEditorHeaders.h>
#include <VisualizerWindowHeaders.h>
#include "RiverOutput.h"

#endif //OPEN_EPHYS_GUI_SCHEMALISTBOX_H
