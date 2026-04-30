#pragma once
#include <vector>

template<typename T, typename R, typename C>
class Validator {
public:
    bool verify_data_types(T* table, R& tableData);
    bool verify_cell_types(C& cell_1, C& cell_2);
    bool verify_column_size(T* table, R& tableData);
};

template <typename T, typename R, typename C>
bool Validator<T, R, C>::verify_data_types(T* table, R& row) {
    if (table->get_meta().data_types[0] == "ANY") { // Escape ANY type
        return true;
    }

    for (int i = 1; i < (int)row.get_cells().size(); i++) {
        if (table->get_meta().data_types[i] != row.get_cells()[i].get_type_name()) {
            return false;
        }
    }

    return true;
}

template <typename T, typename R, typename C>
bool Validator<T, R, C>::verify_cell_types(C& cell_1, C& cell_2) {
    if (cell_1.get_type_name() != cell_2.get_type_name()) {
        return false;
    }

    return true;
}

template <typename T, typename R, typename C>
bool Validator<T, R, C>::verify_column_size(T* table, R& row) {
    if (table->get_meta().column_size != row.get_cells().size()) 
    return false;

    return true;
}