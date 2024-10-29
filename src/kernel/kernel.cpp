/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#include "kernel.h"

Kernel::Kernel() {
    models = new ModelList(this);
    fixtures = new FixtureList(this);
    groups = new GroupList(this);
    intensities = new IntensityList(this);
    colors = new ColorList(this);
    transitions = new TransitionList(this);
    cues = new CueList(this);
    terminal = new Terminal(this);
    inspector = new Inspector(this);
    cuelistView = new CuelistView(this);
    mutex = new QMutex();
}

QString Kernel::execute(QList<int> command, QString text) {
    QMutexLocker locker(mutex);
    QList<int> selection;
    QList<QList<int>> operations;
    if(command.size() < 1) { // if command is empty
        return "Command is empty.";
    }
    int selectionType = command[0];
    command.removeFirst();
    if (!isItem(selectionType)) {
        return "No Item Type specified.";
    }
    for (const int key : command) {
        if (isOperator(key)) { // if a new operation is started
            QList<int> newOperation;
            newOperation.append(key);
            operations.append(newOperation);
        } else {
            if (operations.size() > 0) { // if the selection is finished
                operations[operations.size() - 1].append(key);
            } else {
                selection.append(key);
            }
        }
    }
    if (selection.isEmpty()) {
        return "No selection given.";
    }
    QList<QString> ids = keysToSelection(selection);
    if (ids.isEmpty()) {
        return "Invalid fixture selection.";
    }
    if (operations.size() < 1) {
        return "No operator specified.";
    }
    if (!text.isNull() && operations.size() > 1) {
        return "Passing a text to executeCommand limits the amount of operations to one.";
    }
    for (QList<int> operation : operations) {
        int operationType = operation[0];
        operation.removeFirst();
        if (operationType == Keys::Copy) { // COPY
            QString targetId = keysToId(operation);
            if (targetId.isEmpty()) {
                return "Target ID not valid.";
            }
            QString result;
            if (selectionType == Keys::Model) { // COPY MODEL
                result = models->copyModel(ids, targetId);
            } else if (selectionType == Keys::Fixture) { // COPY FIXTURE
                result = fixtures->copyFixture(ids, targetId);
            } else if (selectionType == Keys::Group) { // COPY GROUP
                result = groups->copyGroup(ids, targetId);
            } else if (selectionType == Keys::Intensity) { // COPY INTENSITY
                result = intensities->copyIntensity(ids, targetId);
            } else if (selectionType == Keys::Color) { // COPY COLOR
                result = colors->copyColor(ids, targetId);
            } else if (selectionType == Keys::Transition) { // COPY TRANSITION
                result = transitions->copyTransition(ids, targetId);
            } else if (selectionType == Keys::Cue) { // COPY CUE
                result = cues->copyCue(ids, targetId);
            } else {
                return "Unknown item type.";
            }
            if (!result.isEmpty()) { // if the operation failed
                return result;
            }
        } else if (operationType == Keys::Delete) { // DELETE
            if (!operation.isEmpty() && !(selectionType ==  Keys::Cue)) {
                return "Delete doesn't take any parameters.";
            }
            QString result;
            if (selectionType == Keys::Model) { // DELETE MODEL
                result = models->deleteModel(ids);
            } else if (selectionType == Keys::Fixture) { // DELETE FIXTURE
                result = fixtures->deleteFixture(ids);
            } else if (selectionType == Keys::Group) { // DELETE GROUP
                result = groups->deleteGroup(ids);
            } else if (selectionType == Keys::Intensity) { // DELETE INTENSITY
                result = intensities->deleteIntensity(ids);
            } else if (selectionType == Keys::Color) { // DELETE COLOR
                result = colors->deleteColor(ids);
            } else if (selectionType == Keys::Transition) { // DELETE TRANSITION
                result = transitions->deleteTransition(ids);
            } else if (selectionType == Keys::Cue) { // DELETE CUE
                if (operation.isEmpty()) {
                    result = cues->deleteCue(ids);
                } else {
                    if (operation[0] != Keys::Group) {
                        return "Delete Cue takes either no parameters or a Group.";
                    }
                    operation.removeFirst();
                    bool intensityDelete = false;
                    bool colorDelete = false;
                    QList<int> groupSelection;
                    for (int key : operation) {
                        if (isNumber(key) || key == Keys::Period) {
                            if (intensityDelete || colorDelete) {
                                return "Can't delete Cue Group: No IDs are permitted after Intensity or Color.";
                            }
                            groupSelection.append(key);
                        } else if (key == Keys::Intensity) {
                            intensityDelete = true;
                        } else if (key == Keys::Color) {
                            colorDelete = true;
                        } else {
                            return "Can't delete Cue Group: Invalid key entered.";
                        }
                    }
                    QString groupId = keysToId(groupSelection);
                    if (groupId.isEmpty()) {
                        return "Can't delete Cue Group: No valid Group ID given.";
                    }
                    if (intensityDelete || (!(intensityDelete || colorDelete))) {
                        QString result = cues->deleteCueGroupIntensity(ids, groupId);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                    if (colorDelete || (!(intensityDelete || colorDelete))) {
                        QString result = cues->deleteCueGroupColor(ids, groupId);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
            } else {
                return "Unknown item type.";
            }
            if (!result.isEmpty()) { // if the operation failed
                return result;
            }
        } else if (operationType == Keys::Label) { // LABEL
            if (!operation.isEmpty()) {
                return "Label doesn't take any parameters.";
            }
            QString label = text;
            if (text.isNull()) {
                bool ok = false;
                locker.unlock();
                label = QInputDialog::getText(terminal, QString(), "Label", QLineEdit::Normal, QString(), &ok);
                locker.relock();
                if (!ok) {
                    return "Popup cancelled.";
                }
            }
            label.replace("\"", "");
            QString result;
            if (selectionType == Keys::Model) { // LABEL MODEL
                result = models->labelModel(ids, label);
            } else if (selectionType == Keys::Fixture) { // LABEL FIXTURE
                result = fixtures->labelFixture(ids, label);
            } else if (selectionType == Keys::Group) { // LABEL GROUP
                result = groups->labelGroup(ids, label);
            } else if (selectionType == Keys::Intensity) { // LABEL INTENSITY
                result = intensities->labelIntensity(ids, label);
            } else if (selectionType == Keys::Color) { // LABEL COLOR
                result = colors->labelColor(ids, label);
            } else if (selectionType == Keys::Transition) { // LABEL TRANSITION
                result = transitions->labelTransition(ids, label);
            } else if (selectionType == Keys::Cue) { // LABEL CUE
                result = cues->labelCue(ids, label);
            } else {
                return "Unknown item type.";
            }
            if (!result.isEmpty()) { // if the operation failed
                return result;
            }
        } else if (operationType == Keys::Move) { // MOVE
            QString targetId = keysToId(operation);
            if (targetId.isEmpty()) {
                return "Target ID not valid.";
            }
            QString result;
            if (selectionType == Keys::Model) { // MOVE MODEL
                result = models->moveModel(ids, targetId);
            } else if (selectionType == Keys::Fixture) { // MOVE FIXTURE
                result = fixtures->moveFixture(ids, targetId);
            } else if (selectionType == Keys::Group) { // MOVE GROUP
                result = groups->moveGroup(ids, targetId);
            } else if (selectionType == Keys::Intensity) { // MOVE INTENSITY
                result = intensities->moveIntensity(ids, targetId);
            } else if (selectionType == Keys::Color) { // MOVE COLOR
                result = colors->moveColor(ids, targetId);
            } else if (selectionType == Keys::Transition) { // MOVE TRANSITION
                result = transitions->moveTransition(ids, targetId);
            } else if (selectionType == Keys::Cue) { // MOVE CUE
                result = cues->moveCue(ids, targetId);
            } else {
                return "Unknown item type.";
            }
            if (!result.isEmpty()) { // if the operation failed
                return result;
            }
        } else if (operationType == Keys::Record) { // RECORD
            if (selectionType == Keys::Model) { // RECORD MODEL
                if (!operation.isEmpty()) {
                    return "Record Model doesn't take any parameters.";
                }
                QString channels = text;
                if (text.isNull()) {
                    bool ok = false;
                    locker.unlock();
                    channels = QInputDialog::getText(terminal, QString(), "Channels", QLineEdit::Normal, QString(), &ok);
                    locker.relock();
                    if (!ok) {
                        return "Popup cancelled.";
                    }
                }
                QString result = models->recordModelChannels(ids, channels);
                if (!result.isEmpty()) {
                    return result;
                }
            } else if (selectionType == Keys::Fixture) { // RECORD FIXTURE
                if (operation.isEmpty()) {
                    return "Record Fixture requires arguments.";
                }
                int address = 0;
                QList<int> model;
                bool addressFinished = false;
                for (int key : operation) {
                    if (isNumber(key) || key == Keys::Period) {
                        if (!addressFinished) {
                            if (!isNumber(key)) {
                                return "DMX Address only allows numbers as input";
                            }
                            address = (address * 10) + keyToNumber(key);
                        } else {
                            model.append(key);
                        }
                    } else if (key == Keys::Model) {
                        if (addressFinished) {
                            return "Record Fixture only takes one Model";
                        }
                        addressFinished = true;
                    } else {
                        return "Record Fixture only takes DMX Address and Model";
                    }
                }
                if (addressFinished) {
                    QString modelId = keysToId(model);
                    if (modelId.isEmpty()) {
                        return "Model selection not valid.";
                    }
                    QString result = fixtures->recordFixtureModel(ids, modelId, address);
                    if (!result.isEmpty()) {
                        return result;
                    }
                } else if (address > 0) {
                    QString result = fixtures->recordFixtureAddress(ids, address);
                    if (!result.isEmpty()) {
                        return result;
                    }
                }
            } else if (selectionType == Keys::Group) { // RECORD GROUP
                QString result;
                if (operation.isEmpty()) {
                    result = groups->recordGroupFixtures(ids, QList<QString>());
                } else {
                    if (operation[0] != Keys::Fixture) {
                        return "Can't record Group because no Fixtures were given.";
                    }
                    operation.removeFirst();
                    QList<QString> fixtureIds = keysToSelection(operation);
                    if (fixtureIds.isEmpty()) {
                        return "Can't record Group because of invalid Fixture selection.";
                    }
                    result = groups->recordGroupFixtures(ids, fixtureIds);
                }
                if (!result.isEmpty()) {
                    return result;
                }
            } else if (selectionType == Keys::Intensity) { // RECORD INTENSITY
                if (operation.isEmpty()) {
                    return "Record Intensity requires arguments.";
                }
                QList<float> values = keysToValue(operation);
                if (values.empty() || values.size() > 1) {
                    return "Invalid values given to Record Intensity.";
                }
                if (values.size() >= 1) {
                    if (values[0] > -999) {
                        QString result = intensities->recordIntensityDimmer(ids, values[0]);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
            } else if (selectionType == Keys::Color) { // RECORD COLOR
                if (operation.isEmpty()) {
                    return "Record Color requires arguments.";
                }
                QList<float> values = keysToValue(operation);
                if (values.empty() || values.size() > 3) {
                    return "Invalid values given to Record Color.";
                }
                if (values.size() >= 1) {
                    if (values[0] > -999) {
                        QString result = colors->recordColorRed(ids, values[0]);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
                if (values.size() >= 2) {
                    if (values[1] > -999) {
                        QString result = colors->recordColorGreen(ids, values[1]);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
                if (values.size() >= 3) {
                    if (values[2] > -999) {
                        QString result = colors->recordColorBlue(ids, values[2]);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
            } else if (selectionType == Keys::Transition) { // RECORD TRANSITION
                if (operation.isEmpty()) {
                    return "Record Transition requires arguments.";
                }
                QList<float> values = keysToValue(operation);
                if (values.empty() || values.size() > 1) {
                    return "Invalid values given to Record Transition.";
                }
                if (values.size() >= 1) {
                    if (values[0] > -999) {
                        QString result = transitions->recordTransitionFade(ids, values[0]);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                }
            } else if (selectionType == Keys::Cue) { // RECORD CUE
                if (operation.isEmpty()) {
                    return "Record Cue requires an argument.";
                }
                if (operation[0] == Keys::Transition) {
                    operation.removeFirst();
                    QString transitionId = keysToId(operation);
                    if (transitionId.isEmpty()) {
                        return "Transition ID not valid.";
                    }
                    QString result = cues->recordCueTransition(ids, transitionId);
                    if (!result.isEmpty()) {
                        return result;
                    }
                } else if (operation[0] == Keys::Group) {
                    operation.removeFirst();
                    operation.append(Keys::Intensity);
                    QString intensityId;
                    QString colorId;
                    QString groupId;
                    int lastItem = Keys::Group;
                    QList<int> selection;
                    for (int key : operation) {
                        if (isNumber(key) || key == Keys::Period) {
                            selection.append(key);
                        } else if (key == Keys::Intensity || key == Keys::Color) {
                            QString selectionId = keysToId(selection);
                            if (selectionId.isEmpty()) {
                                return "Invalid values given to Record Cue.";
                            }
                            selection.clear();
                            if (lastItem == Keys::Group) {
                                groupId = selectionId;
                            } else if (lastItem == Keys::Intensity) {
                                intensityId = selectionId;
                            } else if (lastItem == Keys::Color) {
                                colorId = selectionId;
                            } else {
                                return "Invalid values given to Record Cue.";
                            }
                            lastItem = key;
                        } else {
                            return "Invalid values given to Record Cue.";
                        }
                    }
                    if (groupId.isEmpty()) {
                        return "No Group specified.";
                    }
                    if (!intensityId.isEmpty()) {
                        QString result = cues->recordCueIntensity(ids, groupId, intensityId);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                    if (!colorId.isEmpty()) {
                        QString result = cues->recordCueColor(ids, groupId, colorId);
                        if (!result.isEmpty()) {
                            return result;
                        }
                    }
                } else {
                    return "Record Cue needs either a Group or a Transition as first argument.";
                }
            } else {
                return "Unknown item type.";
            }
        } else {
            return "Unknown operator.";
        }
    }
    cuelistView->loadCue();
    return QString();
}

QString Kernel::keysToId(QList<int> keys)
{
    QString id;
    for (const int key : keys) {
        if (isNumber(key)){
            id += QString::number(keyToNumber(key));
        } else if (key == Keys::Period) {
            id += ".";
        } else {
            return QString();
        }
    }
    while (id.count(".") < 4) {
        id += ".0";
    }
    if (!id.contains(QRegularExpression("^[0-9]+.[0-9]+.[0-9]+.[0-9]+.[0-9]+$"))) {
        return QString();
    }
    return id;
}

QList<float> Kernel::keysToValue(QList<int> keys)
{
    keys.append(Keys::Plus);
    QList<float> values;
    QString value;
    for (const int key : keys) {
        if (isNumber(key)) {
            value += QString::number(keyToNumber(key));
        } else if (key == Keys::Period) {
            if (value.contains(".")) {
                return QList<float>();
            }
            value += ".";
        } else if (key == Keys::Plus) {
            if (value.isEmpty()) {
                values.append(-999);
            } else {
                bool ok;
                float newValue = value.toFloat(&ok);
                if (!ok) {
                    return QList<float>();
                }
                newValue = (floor((newValue * 100) + 0.5) / 100);
                values.append(newValue);
            }
            value = QString();
        } else {
            return QList<float>();
        }
    }
    return values;
}

QList<QString> Kernel::keysToSelection(QList<int> keys)
{
    if (keys.isEmpty()) {
        return QList<QString>();
    }
    if (!keys.endsWith(Keys::Plus)) {
        keys.append(Keys::Plus);
    }
    QList<QString> ids;
    QList<int> idKeys;
    for (int key : keys) {
        if (isNumber(key) || key == Keys::Period) {
            idKeys += key;
        } else if (key == Keys::Plus) {
            QString id = keysToId(idKeys);
            if (id.isEmpty()) {
                return QList<QString>();
            }
            ids.append(id);
            idKeys.clear();
        } else {
            return QList<QString>();
        }
    }
    return ids;
}

bool Kernel::isItem(int key)
{
    return (
        (key == Keys::Model) ||
        (key == Keys::Fixture) ||
        (key == Keys::Group) ||
        (key == Keys::Intensity) ||
        (key == Keys::Color) ||
        (key == Keys::Transition) ||
        (key == Keys::Cue)
    );
}

bool Kernel::isOperator(int key)
{
    return (
        (key == Keys::Copy) ||
        (key == Keys::Delete) ||
        (key == Keys::Label) ||
        (key == Keys::Move) ||
        (key == Keys::Record)
        );
}

bool Kernel::isNumber(int key)
{
    return (
        (key == Keys::Zero) ||
        (key == Keys::One) ||
        (key == Keys::Two) ||
        (key == Keys::Three) ||
        (key == Keys::Four) ||
        (key == Keys::Five) ||
        (key == Keys::Six) ||
        (key == Keys::Seven) ||
        (key == Keys::Eight) ||
        (key == Keys::Nine)
        );
}

int Kernel::keyToNumber(int key)
{
    if (key == Keys::Zero) {
        return 0;
    } else if (key == Keys::One) {
        return 1;
    } else if (key == Keys::Two) {
        return 2;
    } else if (key == Keys::Three) {
        return 3;
    } else if (key == Keys::Four) {
        return 4;
    } else if (key == Keys::Five) {
        return 5;
    } else if (key == Keys::Six) {
        return 6;
    } else if (key == Keys::Seven) {
        return 7;
    } else if (key == Keys::Eight) {
        return 8;
    } else if (key == Keys::Nine) {
        return 9;
    } else {
        return false;
    }
}
