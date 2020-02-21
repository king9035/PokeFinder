/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Eggs3.hpp"
#include "ui_Eggs3.h"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Gen3/ProfileLoader3.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Models/Gen3/EggModel3.hpp>
#include <QSettings>

Eggs3::Eggs3(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

Eggs3::~Eggs3()
{
    QSettings setting;
    setting.beginGroup("eggs3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs3::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs3::profilesIndexChanged);

    profiles = ProfileLoader3::getProfiles();
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("eggs3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs3::setupModels()
{
    emeraldIVs = new EggModel3(ui->tableViewEmeraldIVs, Method::EBred);
    emeraldPID = new EggModel3(ui->tableViewEmeraldPID, Method::EBredPID);
    rs = new EggModel3(ui->tableViewRS, Method::RSBred);
    frlg = new EggModel3(ui->tableViewFRLG, Method::FRLGBred);

    ui->tableViewEmeraldIVs->setModel(emeraldIVs);
    ui->tableViewEmeraldPID->setModel(emeraldPID);
    ui->tableViewRS->setModel(rs);
    ui->tableViewFRLG->setModel(frlg);

    ui->textBoxEmeraldPIDInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxEmeraldPIDMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxMinRedraws->setValues(0, 255, 3, 10);
    ui->textBoxMaxRedraws->setValues(0, 255, 3, 10);
    ui->textBoxCalibration->setValues(0, 255, 3, 10);
    ui->textBoxEmeraldIVsInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxEmeraldIVsMaxResults->setValues(InputType::Frame32Bit);

    ui->textBoxRSSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxRSSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxRSInitialFrameHeld->setValues(InputType::Frame32Bit);
    ui->textBoxRSMaxResultsHeld->setValues(InputType::Frame32Bit);
    ui->textBoxRSInitialFramePickup->setValues(InputType::Frame32Bit);
    ui->textBoxRSMaxResultsPickup->setValues(InputType::Frame32Bit);

    ui->textBoxFRLGSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGInitialFrameHeld->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGMaxResultsHeld->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGInitialFramePickup->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGMaxResultsPickup->setValues(InputType::Frame32Bit);

    ui->comboBoxEmeraldCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxRSCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxFRLGCompatibility->setup({ 20, 50, 70 });

    ui->comboBoxEmeraldGender->setup({ 255, 0, 1 });
    ui->comboBoxRSGender->setup({ 255, 0, 1 });
    ui->comboBoxFRLGGender->setup({ 255, 0, 1 });

    ui->comboBoxEmeraldAbility->setup({ 255, 0, 1 });
    ui->comboBoxRSAbility->setup({ 255, 0, 1 });
    ui->comboBoxFRLGAbility->setup({ 255, 0, 1 });

    ui->comboBoxEmeraldGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });
    ui->comboBoxRSGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });
    ui->comboBoxFRLGGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });

    ui->comboBoxEmeraldNature->setup(Translator::getNatures());
    ui->comboBoxEverstone->addItems(Translator::getNatures());
    ui->comboBoxFRLGNature->setup(Translator::getNatures());
    ui->comboBoxRSNature->setup(Translator::getNatures());

    ui->comboBoxEmeraldHiddenPower->setup(Translator::getHiddenPowers());
    ui->comboBoxFRLGHiddenPower->setup(Translator::getHiddenPowers());
    ui->comboBoxRSHiddenPower->setup(Translator::getHiddenPowers());

    ui->comboBoxEmeraldMethod->setup({ Method::EBred, Method::EBredSplit, Method::EBredAlternate });
    ui->comboBoxRSMethod->setup({ Method::RSBred, Method::RSBredSplit, Method::RSBredAlternate });
    ui->comboBoxFRLGMethod->setup({ Method::FRLGBred, Method::FRLGBredSplit, Method::FRLGBredAlternate });

    connect(ui->pushButtonEmeraldPIDGenerate, &QPushButton::clicked, this, &Eggs3::emeraldPIDGenerate);
    connect(ui->pushButtonEmeraldIVsGenerate, &QPushButton::clicked, this, &Eggs3::emeraldIVsGenerate);
    connect(ui->pushButtonRSGenerate, &QPushButton::clicked, this, &Eggs3::rsGenerate);
    connect(ui->pushButtonFRLGGenerate, &QPushButton::clicked, this, &Eggs3::frlgGenerate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
    connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emeraldIVs, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsRS, &EggSettings::toggleInheritance, rs, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsFRLG, &EggSettings::toggleInheritance, frlg, &EggModel3::toggleInheritance);

    QSettings setting;
    if (setting.contains("eggs3/geometry"))
    {
        this->restoreGeometry(setting.value("eggs3/geometry").toByteArray());
    }
}

void Eggs3::emeraldPIDGenerate()
{
    emeraldPID->clearModel();

    u32 initialFrame = ui->textBoxEmeraldPIDInitialFrame->getUInt();
    u32 maxResults = ui->textBoxEmeraldPIDMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->comboBoxEmeraldGenderRatio->getCurrentByte();

    EggGenerator3 generator(initialFrame, maxResults, tid, sid, genderRatio, Method::EBredPID);
    generator.setMinRedraw(ui->textBoxMinRedraws->getUChar());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->getUChar());
    generator.setCalibration(ui->textBoxCalibration->getUChar());
    generator.setCompatability(ui->comboBoxEmeraldCompatibility->getCurrentByte());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.setEverstoneNature(static_cast<u8>(ui->comboBoxEverstone->currentIndex() - 1));
    }

    FrameFilter filter(ui->comboBoxEmeraldGender->getCurrentByte(), ui->comboBoxEmeraldAbility->getCurrentByte(),
                       ui->checkBoxEmeraldShiny->isChecked(), false, QVector<u8>(), QVector<u8>(), ui->comboBoxEmeraldNature->getChecked(),
                       QVector<bool>(), QVector<bool>());

    auto frames = generator.generate(filter);
    emeraldPID->addItems(frames);
}

void Eggs3::emeraldIVsGenerate()
{
    emeraldIVs->clearModel();

    u32 initialFrame = ui->textBoxEmeraldIVsInitialFrame->getUInt();
    u32 maxResults = ui->textBoxEmeraldIVsMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    auto method = static_cast<Method>(ui->comboBoxEmeraldMethod->getCurrentInt());

    EggGenerator3 generator(initialFrame, maxResults, tid, sid, 0, method);
    generator.setParents(ui->eggSettingsEmerald->getParent1(), ui->eggSettingsEmerald->getParent2());

    FrameFilter filter(0, 0, false, false, ui->ivFilterEmerald->getLower(), ui->ivFilterEmerald->getUpper(), QVector<bool>(),
                       ui->comboBoxEmeraldHiddenPower->getChecked(), QVector<bool>());

    auto frames = generator.generate(filter);
    emeraldIVs->addItems(frames);
}

void Eggs3::rsGenerate()
{
    rs->clearModel();

    u32 initialFrameHeld = ui->textBoxRSInitialFrameHeld->getUInt();
    u32 maxResultsHeld = ui->textBoxRSMaxResultsHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->comboBoxRSGenderRatio->getCurrentByte();
    auto method = static_cast<Method>(ui->comboBoxRSMethod->getCurrentInt());

    EggGenerator3 generator(initialFrameHeld, maxResultsHeld, tid, sid, genderRatio, method);
    generator.setParents(ui->eggSettingsRS->getParent1(), ui->eggSettingsRS->getParent2());
    generator.setCompatability(static_cast<u8>(ui->comboBoxRSCompatibility->currentData().toUInt()));
    generator.setInitialFramePickup(ui->textBoxRSInitialFramePickup->getUInt());
    generator.setMaxResultsPickup(ui->textBoxRSMaxResultsPickup->getUInt());

    FrameFilter filter(ui->comboBoxRSGender->getCurrentByte(), ui->comboBoxRSAbility->getCurrentByte(), ui->checkBoxRSShiny->isChecked(),
                       false, ui->ivFilterRS->getLower(), ui->ivFilterRS->getUpper(), ui->comboBoxRSNature->getChecked(),
                       ui->comboBoxRSHiddenPower->getChecked(), QVector<bool>());

    auto frames = generator.generate(filter, ui->textBoxRSSeedHeld->getUInt(), ui->textBoxRSSeedPickup->getUInt());
    rs->addItems(frames);
}

void Eggs3::frlgGenerate()
{
    frlg->clearModel();

    u32 initialFrameHeld = ui->textBoxFRLGInitialFrameHeld->getUInt();
    u32 maxResultsHeld = ui->textBoxFRLGMaxResultsHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = static_cast<u8>(ui->comboBoxFRLGGenderRatio->currentData().toUInt());
    auto method = static_cast<Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

    EggGenerator3 generator(initialFrameHeld, maxResultsHeld, tid, sid, genderRatio, method);
    generator.setParents(ui->eggSettingsFRLG->getParent1(), ui->eggSettingsFRLG->getParent2());
    generator.setCompatability(static_cast<u8>(ui->comboBoxFRLGCompatibility->currentData().toUInt()));
    generator.setInitialFramePickup(ui->textBoxFRLGInitialFramePickup->getUInt());
    generator.setMaxResultsPickup(ui->textBoxFRLGMaxResultsPickup->getUInt());

    FrameFilter filter(static_cast<u8>(ui->comboBoxFRLGGender->currentIndex()), static_cast<u8>(ui->comboBoxFRLGAbility->currentIndex()),
                       ui->checkBoxFRLGShiny->isChecked(), false, ui->ivFilterFRLG->getLower(), ui->ivFilterFRLG->getUpper(),
                       ui->comboBoxFRLGNature->getChecked(), ui->comboBoxFRLGHiddenPower->getChecked(), QVector<bool>());

    auto frames = generator.generate(filter, ui->textBoxFRLGSeedHeld->getUInt(), ui->textBoxFRLGSeedPickup->getUInt());
    frlg->addItems(frames);
}

void Eggs3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
    }
}

void Eggs3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
