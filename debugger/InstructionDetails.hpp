/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_INSTRUCTIONDETAILS_HPP
#define MICROMACHINE_EMU_INSTRUCTIONDETAILS_HPP

#include <QObject>
#include <QQmlListProperty>
#include <QSharedPointer>
#include <vector>

class InstructionComponent : public QObject {
	Q_OBJECT
	Q_PROPERTY(component_type type READ componentType CONSTANT)
	Q_PROPERTY(QString componentName READ componentName CONSTANT)
public:
	enum component_type {
		name = 0,
		register_identifier = 1,
		special_register_identifier = 2,
		immediate_value = 3,
		register_list = 4,
		condition_flag_identifier = 5,
		label_address = 6,
		begin_deref_group = 7,
		end_deref_group = 8,
	};
	Q_ENUM(component_type)

	InstructionComponent(
		component_type type,
		const std::string& name = "",
		uint8_t reg_id = 0,
		const std::string& special_reg = "",
		uint32_t immediate = 0,
		uint32_t label_adress = 0,
		const std::vector<uint8_t>& reg_list = {})
		: QObject(nullptr)
		, _type(type)
		, _name(name)
		, _reg_id(reg_id)
		, _special_reg(special_reg)
		, _immediate_value(immediate)
		, _label_address(label_adress)
		, _reg_list(reg_list)
		{}

	InstructionComponent() = default;

	void clear() {
		_reg_list.clear();
	}

	component_type componentType() const {
		return _type;
	}

	QString componentName() const {
		return QString::fromStdString(_name);
	}

	component_type _type;
	std::string _name = "";
	uint8_t _reg_id = 0;
	std::string _special_reg = "";
	uint32_t _immediate_value = 0;
	uint32_t _label_address = 0;
	std::vector<uint8_t> _reg_list = {};
};


namespace {
	int componentCount(QQmlListProperty<InstructionComponent>* list_property) {
		std::vector<QSharedPointer<InstructionComponent>>* vec
		= (std::vector<QSharedPointer<InstructionComponent>>*)list_property->data;
		return vec->size();
	}

	InstructionComponent* componentAt(QQmlListProperty<InstructionComponent>* list_property, int index) {
		std::vector<QSharedPointer<InstructionComponent>>* vec
		= (std::vector<QSharedPointer<InstructionComponent>>*)list_property->data;
		return vec->at(index).data();
	}
}

class InstructionDetails : public QObject {
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<InstructionComponent> components READ components CONSTANT)
public:
	const uint32_t _address = 0;
	std::vector<QSharedPointer<InstructionComponent>> _components;

	void clear() {
		_components.clear();
	}

	QQmlListProperty<InstructionComponent> components() {
		return QQmlListProperty<InstructionComponent>(
			this,
			&_components,
			&componentCount,
			&componentAt
		);
	}

	InstructionDetails& add_name(const std::string& name) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::name, name));
		return *this;
	}

	InstructionDetails& add_reg(uint8_t reg_id) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::register_identifier,"", reg_id));
		return *this;
	}

	InstructionDetails& add_special_reg(const std::string& reg_name) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::special_register_identifier,"", 0,
		reg_name));
		return *this;
	}

	InstructionDetails& add_immediate(uint32_t immediate) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::immediate_value,"", 0, "", immediate));
		return *this;
	}

	InstructionDetails& add_regs(const std::vector<uint8_t>& regs) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::register_list,"", 0, "", 0, 0, regs));
		return *this;
	}

	InstructionDetails& add_condition_flag(const std::string& name) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::condition_flag_identifier, name));
		return *this;
	}

	InstructionDetails& add_label_address(uint32_t label_address) {
		_components.emplace_back(new InstructionComponent(InstructionComponent::label_address, "", 0, "", 0, label_address));
		return *this;
	}

	InstructionDetails& begin_deref() {
		_components.emplace_back(new InstructionComponent(InstructionComponent::begin_deref_group));
		return *this;
	}

	InstructionDetails& end_deref() {
		_components.emplace_back(new InstructionComponent(InstructionComponent::end_deref_group));
		return *this;
	}
};

#endif //MICROMACHINE_EMU_INSTRUCTIONDETAILS_HPP
