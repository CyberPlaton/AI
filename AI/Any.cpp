#include"Any.h"


bool Any::_equal(std::any data) {

	if (strcmp(m_Value.type().name(), data.type().name()) == 0) {

		switch (m_Type) {
		case ANY_INT:
			return std::any_cast<int>(data) == std::any_cast<int>(m_Value);

		case ANY_DOUBLE:
			return std::any_cast<double>(data) == std::any_cast<double>(m_Value);

		case ANY_FLOAT:
			return std::any_cast<float>(data) == std::any_cast<float>(m_Value);

		case ANY_STRING:
			// Comparing strings is somewhat special.
			return (COMPARE_STRINGS_2(std::any_cast<std::string>(data), std::any_cast<std::string>(m_Value)) == 0);
		}
	}

	return false;
}


bool Any::_smaller(std::any data) {

	if (strcmp(m_Value.type().name(), data.type().name()) == 0) {

		switch (m_Type) {
		case ANY_INT:
			return std::any_cast<int>(data) < std::any_cast<int>(m_Value);

		case ANY_DOUBLE:
			return std::any_cast<double>(data) < std::any_cast<double>(m_Value);

		case ANY_STRING:
			// Comparing strings is somewhat special.
			return (COMPARE_STRINGS_2(std::any_cast<std::string>(data), std::any_cast<std::string>(m_Value)) < 0);

		case ANY_FLOAT:
			return std::any_cast<float>(data) < std::any_cast<float>(m_Value);

		}
	}

	return false;
}


bool Any::_greater(std::any data) {

	if (strcmp(m_Value.type().name(), data.type().name()) == 0) {

		switch (m_Type) {
		case ANY_INT:
			return std::any_cast<int>(data) == std::any_cast<int>(m_Value);

		case ANY_DOUBLE:
			return std::any_cast<double>(data) == std::any_cast<double>(m_Value);

		case ANY_STRING:
			// Comparing strings is somewhat special.
			return (COMPARE_STRINGS_2(std::any_cast<std::string>(data), std::any_cast<std::string>(m_Value)) == 0);

		case ANY_FLOAT:
			return std::any_cast<float>(data) == std::any_cast<float>(m_Value);
		}
	}

	return false;
}