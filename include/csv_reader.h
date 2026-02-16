#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class CSVreader {
    std::vector<std::vector<std::string>> data;
public:
    CSVreader () = default;
    friend std::istream& operator>>(std::istream& in, CSVreader& reader) {
        std::string line;
        std::cout <<"File is open!\n";
        while (std::getline(in, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            if (!line.empty() && line.back() == ',') {
                row.push_back("");
            }
            reader.data.push_back(row);
        }
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, CSVreader& reader) {
        for (const auto& row : reader.data) {
            for (int i = 0;i < row.size();++i) {
                if (i != 0) {
                    out << "\t";
                }
                out << row[i];
            }
            out << std::endl;
        }
        return out;
    }
    std::vector<int> countNonEmptyCellsPerColumn() const {
        int countColumns = 0;
        for (const auto& row : data) {
            countColumns = std::max(countColumns, (int)row.size());
        }
        std::vector<int> ans(countColumns, 0);
        for (const auto& row : data) {
            for (int i = 0;i < row.size();++i) {
                if (!row.empty()) {
                    ans[i] += 1;
                }
            }
        }
        return ans;
    }
};
