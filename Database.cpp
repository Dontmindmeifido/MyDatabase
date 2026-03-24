#include <iostream>
#include <vector>

using namespace std;

bool isNUMBER(string value) {
    bool isNumber = true;
    for (auto x: value) {
        if (!(char(x) >= 48 && char(x) <= 57)) {
            isNumber = false;
        }
    }

    return isNumber;
}

bool isNUMBER(char value) {
    bool isNumber = true;
    if (!(char(value) >= 48 && char(value) <= 57)) {
        isNumber = false;
    }

    return isNumber;
}

bool isDATETIME(string value) {
    bool isDatetime = true;
    vector<char> FORMAT = {'D', 'D', '-', 'M', 'M', '-', 'Y', 'Y', 'Y', 'Y'};

    for (int i = 0; i < FORMAT.size(); i++) {
        switch(FORMAT[i]) {
            case 'D':
                if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case 'M':
                    if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case 'Y':
                if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case '-':
                if (value[i] != '-') {
                    isDatetime = false;
                }
                break;
        }
    }

    return isDatetime;
}

bool isDURATION(string value) {
    bool isDuration = true;
    vector<char> FORMAT = {'D', 'D', '-', 'M', 'M', '-', 'Y', 'Y'};

    for (int i = 0; i < FORMAT.size(); i++) {
        switch(FORMAT[i]) {
            case 'D':
                if (!isNUMBER(value[i])) isDuration = false;
                break;
            case 'M':
                    if (!isNUMBER(value[i])) isDuration = false;
                break;
            case 'Y':
                if (!isNUMBER(value[i])) isDuration = false;
                break;
            case '-':
                if (value[i] != '-') {
                    isDuration = false;
                }
                break;
        }
    }

    return isDuration;
}

class DataType {
protected:
    string value;
    string type;

public:
    string getValue() {
        return this->value;
    }

    string getType() {
        return this->type;
    }

    virtual ~DataType() {};
};

class VARCHAR: public DataType {
public:
    VARCHAR() {
        this->value = "";
        this->type = "VARCHAR";
    }

    VARCHAR(string value) {
        this->value = value;
        this->type = "VARCHAR";
    }

    VARCHAR(VARCHAR* obj) {
        this->value = obj->getValue();
        this->type = "VARCHAR";
    }
};

class NUMBER: virtual public DataType {
public: 
    NUMBER() {
        this->value = "0";
        this->type = "NUMBER";
    }

    NUMBER(string value) {
        if (isNUMBER(value)) {
            this->value = value;
            this->type = "NUMBER";
        } else {
            cout << "ERROR WRONG NUMBER TYPE";
        }
    }

    NUMBER(NUMBER* obj) {
        this->value = obj->getValue();
        this->type = "NUMBER";
    }
};

class DATETIME: virtual public DataType {
public:
    DATETIME() {
        this->value = "01-01-1970";
        this->type = "DATETIME";
    }

    DATETIME(string value) {
        if (isDATETIME(value)) {
            this->value = value;
            this->type = "DATETIME";
        } else {
            cout << "ERROR WRONG DATETIME TYPE";
        }
    }

    DATETIME(DATETIME* obj) {
        this->value = obj->getValue();
        this->type = "DATETIME";
    }
};

class DURATION: public NUMBER, public DATETIME {
public:
    DURATION() {
        this->value = "00-00-00";
        this->type = "DURATION";
    }

    DURATION(string value) {
        if (isDURATION(value)) {
            this->value = value;
            this->type = "DURATION";
        } else {
            cout << "ERROR WRONG DURATION TYPE";
        }
    }

    DURATION(DURATION* obj) {
        this->value = obj->getValue();
        this->type = "DURATION";
    }
};

class Cell {
    DataType* data;

public:
    Cell() {
        this->data = new VARCHAR();
    }

    Cell(const Cell& other) {
        if (other.getData()->getType() == "VARCHAR") {
            this->data = new VARCHAR(other.getData()->getValue());
        } else if (other.getData()->getType() == "NUMBER") {
            this->data = new NUMBER(other.getData()->getValue());
        } else if (other.getData()->getType() == "DATETIME") {
            this->data = new DATETIME(other.getData()->getValue());
        }  else if (other.getData()->getType() == "DURATION") {
            this->data = new DURATION(other.getData()->getValue());
        }
    }

    Cell(string value) {
        if (isDATETIME(value)) {
            this->data = new DATETIME(value);
        } else if (isNUMBER(value)) {
            this->data = new NUMBER(value);
        } else if (isDURATION(value)) {
            this->data = new DURATION(value);
        } else {
            this->data = new VARCHAR(value);
        }
    }

    Cell& operator=(const Cell& other) {
        if (&other == this) {
            return *this;
        }

        delete this->data;

        if (other.getData()->getType() == "VARCHAR") {
            this->data = new VARCHAR(other.getData()->getValue());
        } else if (other.getData()->getType() == "NUMBER") {
            this->data = new NUMBER(other.getData()->getValue());
        } else if (other.getData()->getType() == "DATETIME") {
            this->data = new DATETIME(other.getData()->getValue());
        }  else if (other.getData()->getType() == "DURATION") {
            this->data = new DURATION(other.getData()->getValue());
        }

        return *this;
    }

    string getValue() {
        return this->data->getValue();
    }

    DataType* getData() {
        return this->data;
    }

    DataType* getData() const {
        return this->data;
    }

    ~Cell() {
        delete this->data;
    }
};

class Row {
    vector<Cell> rowData;

public:
    Row(vector<string> rowData) {
        vector<Cell> auxRowData;

        for (auto x: rowData) {
            auxRowData.push_back(Cell(x));
        }

        this->rowData = auxRowData;
    }

    vector<Cell>& getRowData() {
        return this->rowData;
    }

    void setRowData(vector<Cell> rowData) {
        this->rowData = rowData;
    }
};

class Table {
    vector<Row> tableData;
    vector<string> tableDataTypes;
    string tableName;

public:
    Table(vector<string> header, vector<string> datatypes, string tableName) {
        this->tableData.push_back(Row(header));
        this->tableDataTypes = datatypes;
        this->tableName = tableName;
    }

    bool verifyDataTypes(Row tableData) {
        for (int i = 0; i < tableDataTypes.size(); i++) {
            if (tableDataTypes[i] != tableData.getRowData()[i].getData()->getType()) {
                return false;
            }
        }

        return true;
    }

    vector<Row>& getTableData() {
        return this->tableData;
    }

    void setTableData(vector<Row> tableData) {
        this->tableData = tableData;
    }

    string getTableName() {
        return this->tableName;
    }
};

class Database {
    static Database* instance;
    vector<Table> databaseData; 
    static Table* nullTbl;

    Database() {}
public:
    static Database* getInstance() {
        if (!instance) instance = new Database();
        return instance;
    }

    Table& getTable(string name) {
        for (int i = 0; i < databaseData.size(); i++) {
            if (databaseData[i].getTableName() == name) {
                return databaseData[i];
            }
        }

        cout << "TABLE NOT FOUND";
        return *nullTbl;
    }

    vector<Table>& getDatabaseData() {
        return this->databaseData;
    }

    Table* getNullTable() {
        return this->nullTbl;
    }

    void setDatabaseData(vector<Table> databaseData) {
        this->databaseData = databaseData;
    }

    ~Database() {
        delete getInstance();
    }
};

Database* Database::instance = nullptr;
Table* Database::nullTbl = new Table({"NULL_TABLE"}, {"NULL_TABLE"}, "NULL_TABLE");