/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_DISASMVIEW_HPP
#define MICROMACHINE_EMU_DISASMVIEW_HPP



#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QFontDatabase>

#include "QMemRegion.hpp"
#include "QInstruction.hpp"
#include "structured_disasm.hpp"

class Disassembler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QMemRegion* memoryRegion READ memoryRegion WRITE setMemoryRegion NOTIFY regionChanged)
    Q_PROPERTY(QQmlListProperty<Instruction> instructions READ instructions NOTIFY instructionsChanged)
    Q_PROPERTY(quint32 paddingInstructionCount READ paddingInstructionCount WRITE setPaddingInstructionCount NOTIFY paddingInstructionCountChanged)
    Q_PROPERTY(quint32 centerAddress READ centerAddress WRITE setCenterAddress NOTIFY centerAddressChanged)
public:

    Disassembler(QObject* parent = nullptr)
        : QObject(parent)
        , mMem(nullptr)
        , mPaddingInstructionCount(0)
        , mCenterAddress(0)
    {
        connect(this, &Disassembler::paddingInstructionCountChanged, this, &Disassembler::update);
        connect(this, &Disassembler::centerAddressChanged, this, &Disassembler::update);
    }

    void setCenterAddress(uint32_t address) {
        if(mCenterAddress != address) {
            mCenterAddress = address;
            emit centerAddressChanged();
        }
    }

    uint32_t centerAddress() const {
        return mCenterAddress;
    }

    void setPaddingInstructionCount(quint32 count) {
        if(count != mPaddingInstructionCount) {
            mPaddingInstructionCount = count;
            emit paddingInstructionCountChanged();
        }
    }

    quint32 paddingInstructionCount() const {
        return mPaddingInstructionCount;
    }

    QQmlListProperty<Instruction> instructions() {
        return QQmlListProperty<Instruction>(this, this,
            &Disassembler::instructionCount,
            &Disassembler::instruction
        );
    }

    void setMemoryRegion(QMemRegion* region) {
        if(mMem != region) {
            if(mMem) {
                disconnect(mMem, &QMemRegion::memoryChanged, this, &Disassembler::update);
            }
            mMem = region;
            connect(mMem, &QMemRegion::memoryChanged, this, &Disassembler::update);
            emit regionChanged();
        }
    }

    QMemRegion* memoryRegion() {
        return mMem;
    }

signals:

    void regionChanged();
    void instructionsChanged();
    void paddingInstructionCountChanged();
    void centerAddressChanged();

public slots:

    void scrollUp() {
        if(mPaddingInstructionCount < 1) return;
        if(mInstructions.size() < 3) return;
        uint32_t prevInstructionSize = mInstructions.at(mPaddingInstructionCount - 1)->size();
        setCenterAddress(centerAddress() - prevInstructionSize);
    }

    void scrollDown() {
        if(mPaddingInstructionCount < 1) return;
        if(mInstructions.size() < 3) return;
        uint32_t prevInstructionSize = mInstructions.at(mPaddingInstructionCount + 1)->size();
        setCenterAddress(centerAddress() + prevInstructionSize);
    }


    void update() {
        if(mMem == nullptr) return;
        if(0 == mPaddingInstructionCount) return;

        size_t totalNumberOfInstructions = (mPaddingInstructionCount * 2) + 1;
        if((ssize_t)totalNumberOfInstructions < 0) {
            return;
        }

        mInstructions.resize(totalNumberOfInstructions);

        for(auto& instr : mInstructions) {
            if(instr.isNull()) {
                instr = QSharedPointer<Instruction>(new Instruction, &QObject::deleteLater);
            }
        }

        uint32_t nextAddress = mCenterAddress - (totalNumberOfInstructions / 2)*2;
        for(int i = 0; i < totalNumberOfInstructions; i++) {
            QSharedPointer<Instruction> instr = mInstructions.at(i);
            instr->setAddress(nextAddress);
            if(mMem->isValidVirtualAddress(nextAddress)) {
                uint32_t data = 0; // binary representation
                uint32_t size = disassembleAddress(nextAddress, instr->details(), data);
                instr->setSize(size);
                instr->setData(data);
            } else {
                instr->setCode("");
                instr->setSize(2);
                instr->setData(0);
            }
            nextAddress += instr->size();
        }

        emit instructionsChanged();
    }


private:

    int instructionCount() {
        return mInstructions.size();
    }

    static int instructionCount(QQmlListProperty<Instruction>* list) {
        return reinterpret_cast<Disassembler*>((list->data))->instructionCount();
    }

    Instruction* instruction(int index) {
        if(index < instructionCount()) {
            return mInstructions.at(index).data();
        }
        return &mEmptyInstruction;
    }

    static Instruction* instruction(QQmlListProperty<Instruction>* list, int index) {
        return reinterpret_cast<Disassembler*>((list->data))->instruction(index);
    }

    uint32_t disassembleAddress(uint32_t address, InstructionDetails& disasm, uint32_t& data) {
        uint32_t offset = mMem->virtualAddressOffset(address);
        const uint8_t* buffer = mMem->hostMemoryBase();
        halfword first_instr = *(uint16_t*)(buffer + offset); // always prefetch
        halfword second_instr = *(uint16_t*)(buffer + offset + sizeof(halfword)); // always prefetch
        instruction_pair instr = instruction_pair(first_instr, second_instr);
        data = instr.second << 16 | instr.first;
		structured_disasm d(address, DISASM_FMT_HEX, disasm);
		d.dispatch_instruction(instr);
        return instr.size();
    }

    QMemRegion* mMem;
    quint32 mPaddingInstructionCount;
    uint32_t mCenterAddress;
    std::vector<QSharedPointer<Instruction>> mInstructions;
    Instruction mEmptyInstruction;
};

#endif //MICROMACHINE_EMU_DISASMVIEW_HPP
