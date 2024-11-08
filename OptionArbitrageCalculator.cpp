#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class OptionArbitrageCalculator {
public:
    static std::vector<double> opt(double spotPrice, double strikePrice, double optionPrice, char optionType) {
        std::vector<double> arr(2);
        double intrinsicValue;
        int n = 1;
        if (optionType == 'C') {
            intrinsicValue = std::max(0.0, spotPrice - strikePrice);
            n = 0;
        } else if (optionType == 'P') {
            intrinsicValue = std::max(0.0, strikePrice - spotPrice);
            n = 2;
        } else {
            return {-1}; // Invalid option type
        }

        double extrinsicValue = optionPrice - intrinsicValue;
        extrinsicValue = extrinsicValue > 0 ? extrinsicValue : 0;
        arr[0] = extrinsicValue;
        arr[1] = intrinsicValue;
        return arr;
    }

    void calculatePnL(std::vector<std::vector<double>> optionChain, std::vector<double> fut, double spotPrice, int curr_date, int opt_exp, int fut_exp, int ty) {
        double fut_bid = fut[0];
        double fut_ask = fut[1];

        double spot_b = 0, spot_s = 0;
        spot_s = (fut_bid - spotPrice) * ((fut_exp - curr_date) - (opt_exp - curr_date)) / (opt_exp - curr_date);
        spot_b = (spotPrice - fut_ask) * ((fut_exp - curr_date) - (opt_exp - curr_date)) / (opt_exp - curr_date);

        std::vector<std::vector<double>> opt_data(optionChain.size(), std::vector<double>(11));
        int cnt = 0;
        double callBid, callAsk, strikePrice, putBid, putAsk;
        for (const auto& option : optionChain) {
            callBid = option[0];
            callAsk = option[1];
            strikePrice = option[2];
            putBid = option[3];
            putAsk = option[4];

            double pnlConversionAbove = strikePrice - spotPrice;
            double pnlConversionAt3 = putAsk;
            double pnlConversionAt4 = callBid;
            double pnlConversionAt5 = opt(spotPrice, strikePrice, putAsk, 'P')[0];
            double pnlConversionAt6 = opt(spotPrice, strikePrice, callBid, 'C')[0];
            double pnlConversionAt7 = opt(spotPrice, strikePrice, putAsk, 'P')[1];
            double pnlConversionAt8 = opt(spotPrice, strikePrice, callBid, 'C')[1];
            double pnlConversionAt9 = -pnlConversionAt5 + pnlConversionAt6;
            double pnlConversionAt10 = spot_b;
            double pnlConversionAt11 = spot_b + pnlConversionAt9;

            double pnlReversalAbove = spotPrice - strikePrice;
            double pnlReversalAt3 = callAsk;
            double pnlReversalAt4 = putBid;
            double pnlReversalAt5 = opt(spotPrice, strikePrice, callAsk, 'C')[0];
            double pnlReversalAt6 = opt(spotPrice, strikePrice, putBid, 'P')[0];
            double pnlReversalAt7 = opt(spotPrice, strikePrice, callAsk, 'C')[1];
            double pnlReversalAt8 = opt(spotPrice, strikePrice, putBid, 'P')[1];
            double pnlReversalAt9 = -pnlConversionAt5 + pnlConversionAt6;
            double pnlReversalAt10 = spot_s;
            double pnlReversalAt11 = spot_s + pnlReversalAt9;

            if (ty == 5) {
                opt_data[cnt][0] = strikePrice;
                opt_data[cnt][1] = pnlConversionAbove;
                opt_data[cnt][2] = pnlConversionAt3;
                opt_data[cnt][3] = pnlConversionAt4;
                opt_data[cnt][4] = pnlConversionAt5;
                opt_data[cnt][5] = pnlConversionAt6;
                opt_data[cnt][6] = pnlConversionAt7;
                opt_data[cnt][7] = pnlConversionAt8;
                opt_data[cnt][8] = pnlConversionAt9;
                opt_data[cnt][9] = pnlConversionAt10;
                opt_data[cnt][10] = pnlConversionAt11;
            } else if (ty == 6) {
                opt_data[cnt][0] = strikePrice;
                opt_data[cnt][1] = pnlReversalAbove;
                opt_data[cnt][2] = pnlReversalAt3;
                opt_data[cnt][3] = pnlReversalAt4;
                opt_data[cnt][4] = pnlReversalAt5;
                opt_data[cnt][5] = pnlReversalAt6;
                opt_data[cnt][6] = pnlReversalAt7;
                opt_data[cnt][7] = pnlReversalAt8;
                opt_data[cnt][8] = pnlReversalAt9;
                opt_data[cnt][9] = pnlReversalAt10;
                opt_data[cnt][10] = pnlReversalAt11;
            }
            cnt++;
        }
        print(opt_data);
    }

    void print(const std::vector<std::vector<double>>& a) {
        std::cout << std::endl;
        for (const auto& row : a) {
            for (double value : row)
                std::cout << value << " | ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

// Main function outside the class
int main() {
    // Sample option chain (Call Bid, Call Ask, Strike Price, Put Bid, Put Ask)
    std::vector<std::vector<double>> optionChain = {
        {1005.00, 1018.50, 74000.00, 467.50, 470.00},
        {679.00, 1171.00, 74100.00, 196.50, 1161.50},
        {754.00, 1010.00, 74200.00, 422.50, 1120.50},
        {566.00, 1025.00, 74300.00, 405.50, 1212.50},
        {475.00, 925.00, 74400.00, 417.50, 1570.50},
        {744.00, 758.00, 74500.00, 700.00, 710.50},
        {618.50, 826.50, 74600.00, 711.50, 1296.50},
        {498.50, 787.50, 74700.00, 384.00, 1749.00},
        {535.00, 895.00, 74800.00, 677.50, 1690.00},
        {500.50, 784.00, 74900.00, 709.50, 1587.50},
        {545.50, 548.00, 75000.00, 990.00, 1006.50},
        {379.00, 621.00, 75100.00, 736.50, 1740.00},
        {298.50, 576.50, 75200.00, 1079.50, 1534.50},
        {278.50, 601.50, 75300.00, 700.50, 2099.50}
    };

    double fut_bid = 74520;
    double fut_ask = 74521;

    double spot_price = 74520;
    std::vector<double> fut = {fut_bid, fut_ask};

    int curr_date = 8, opt_exp = 17, fut_exp = 21;

    OptionArbitrageCalculator obj;
    obj.calculatePnL(optionChain, fut, spot_price, curr_date, opt_exp, fut_exp, 5);
    obj.calculatePnL(optionChain, fut, spot_price, curr_date, opt_exp, fut_exp, 6);

    return 0;
}
