/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.net                                                                                             */
/*                                                                                                              */
/*   T E S T I N G   A N A L Y S I S   T E S T   C L A S S                                                      */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   Artelnics - Making intelligent use of data                                                                 */
/*   robertolopez@artelnics.com                                                                                 */
/*                                                                                                              */
/****************************************************************************************************************/

// Unit testing includes

#include "testing_analysis_test.h"

using namespace OpenNN;


TestingAnalysisTest::TestingAnalysisTest(void) : UnitTesting() 
{
}


TestingAnalysisTest::~TestingAnalysisTest(void)
{
}


void TestingAnalysisTest::test_constructor(void)
{
   message += "test_constructor\n";

   // Neural network constructor

   NeuralNetwork nn2;
   TestingAnalysis ta2(&nn2);

   assert_true(ta2.get_neural_network_pointer() != NULL, LOG);

   // Data set constructor

   DataSet ds3;
   TestingAnalysis ta3(&ds3);

   assert_true(ta3.get_data_set_pointer() != NULL, LOG);

   // Neural network and data set constructor

   NeuralNetwork nn4;
   DataSet ds4;

   TestingAnalysis ta4(&nn4, &ds4);

   assert_true(ta4.get_neural_network_pointer() != NULL, LOG);
   assert_true(ta4.get_data_set_pointer() != NULL, LOG);
}


void TestingAnalysisTest::test_destructor(void)
{
   message += "test_destructor\n";
}


void TestingAnalysisTest::test_get_neural_network_pointer(void)
{
   message += "test_get_neural_network_pointer\n";

   TestingAnalysis ta;

   NeuralNetwork nn;

   ta.set_neural_network_pointer(&nn);
   
   assert_true(ta.get_neural_network_pointer() != NULL, LOG);
}


void TestingAnalysisTest::test_get_data_set_pointer(void)
{
   message += "test_get_data_set_pointer\n";

   TestingAnalysis ta;

   DataSet ds;

   ta.set_data_set_pointer(&ds);
   
   assert_true(ta.get_data_set_pointer() != NULL, LOG);
}


void TestingAnalysisTest::test_get_display(void)
{
   message += "test_get_display\n";
}


void TestingAnalysisTest::test_set_neural_network_pointer(void)
{
   message += "test_set_neural_network_pointer\n";
}


void TestingAnalysisTest::test_set_data_set_pointer(void)
{
   message += "test_set_data_set_pointer\n";
}


void TestingAnalysisTest::test_set_display(void)
{
   message += "test_set_display\n";
}


void TestingAnalysisTest::test_calculate_target_output_data(void)
{
   message += "test_calculate_target_output_data\n";

   NeuralNetwork nn;
   DataSet ds;

   TestingAnalysis ta(&nn, &ds);

   Vector< Matrix<double> > target_output_data;

   // Test

   nn.set(1, 1);
   nn.initialize_parameters(0.0);

   ds.set(1, 1, 1);
   ds.initialize_data(0.0);
   ds.get_instances_pointer()->set_testing();

   target_output_data = ta.calculate_target_output_data();

   assert_true(target_output_data.size() == 1, LOG);

   assert_true(target_output_data[0].get_rows_number() == 1, LOG);
   assert_true(target_output_data[0].get_columns_number() == 2, LOG);
   assert_true(target_output_data[0] == 0.0, LOG);
}


void TestingAnalysisTest::test_calculate_error_data(void)
{
    message += "test_calculate_error_data\n";

    NeuralNetwork nn;

    DataSet ds;
    TestingAnalysis ta(&nn, &ds);

    Vector< Matrix<double> > error_data;

    // Test

    nn.set(1, 1);
    nn.construct_unscaling_layer();
    nn.initialize_parameters(0.0);

    ds.set(1, 1, 1);
    ds.initialize_data(0.0);
    ds.get_instances_pointer()->set_testing();

    error_data = ta.calculate_error_data();

    assert_true(error_data.size() == 1, LOG);
    assert_true(error_data[0].get_rows_number() == 1, LOG);
    assert_true(error_data[0].get_columns_number() == 3, LOG);
    assert_true(error_data[0] == 0.0, LOG);

}


void TestingAnalysisTest::test_calculate_error_data_statistics(void)
{
    message += "test_calculate_error_data_statistics\n";

    NeuralNetwork nn;
    DataSet ds;
    TestingAnalysis ta(&nn, &ds);

    Vector< Vector< Statistics<double> > > error_data_statistics;

    // Test

    nn.set(1, 1);
    nn.construct_unscaling_layer();
    nn.initialize_parameters(0.0);

    ds.set(1, 1, 1);
    ds.get_instances_pointer()->set_testing();
    ds.initialize_data(0.0);

    error_data_statistics = ta.calculate_error_data_statistics();

    assert_true(error_data_statistics.size() == 1, LOG);
    assert_true(error_data_statistics[0].size() == 3, LOG);
    assert_true(error_data_statistics[0][0].minimum == 0.0, LOG);
    assert_true(error_data_statistics[0][0].maximum == 0.0, LOG);
    assert_true(error_data_statistics[0][0].mean == 0.0, LOG);
    assert_true(error_data_statistics[0][0].standard_deviation == 0.0, LOG);
}


void TestingAnalysisTest::test_calculate_error_data_statistics_matrices(void)
{
    message += "test_calculate_error_data_statistics_matrices\n";
}


void TestingAnalysisTest::test_calculate_error_data_histograms(void)
{
    message += "test_calculate_error_data_histograms\n";

    NeuralNetwork nn;
    DataSet ds;
    TestingAnalysis ta(&nn, &ds);

    Vector< Histogram<double> > error_data_histograms;

    // Test

    nn.set(1, 1);
    nn.construct_unscaling_layer();
    nn.initialize_parameters(0.0);

    ds.set(1, 1, 3);
    ds.get_instances_pointer()->set_testing();
    ds.initialize_data(0.0);

    error_data_histograms = ta.calculate_error_data_histograms();

    assert_true(error_data_histograms.size() == 1, LOG);
    assert_true(error_data_histograms[0].get_bins_number() == 10, LOG);
}


void TestingAnalysisTest::test_calculate_linear_regression_parameters(void)
{
   message += "test_calculate_linear_regression_parameters\n";

   NeuralNetwork nn;

   DataSet ds;

   Matrix<double> data;

   TestingAnalysis ta(&nn, &ds);

   Vector< LinearRegressionParameters<double> > linear_regression_parameters;

    // Test

   nn.set(1,1);

   nn.initialize_parameters(0.0);

   ds.set(1,1,1);

   ds.get_instances_pointer()->set_testing();

   ds.initialize_data(0.0);

   linear_regression_parameters = ta.calculate_linear_regression_parameters();

   assert_true(linear_regression_parameters.size() == 1, LOG);
}


void TestingAnalysisTest::test_print_linear_regression_parameters(void)
{
   message += "test_print_linear_regression_parameters\n";

   NeuralNetwork nn(1,1,1);

   nn.initialize_parameters(0.0);

   DataSet ds(1,1,1);

   ds.get_instances_pointer()->set_testing();

   ds.initialize_data(0.0);

   TestingAnalysis ta(&nn, &ds);
//   ta.print_linear_regression_parameters();
}


void TestingAnalysisTest::test_save_linear_regression_parameters(void)
{
   message += "test_save_linear_regression_parameters\n";

   std::string file_name = "../data/linear_regression_parameters.dat";

   NeuralNetwork nn;
   DataSet ds;

   TestingAnalysis ta(&nn, &ds);
//   ta.save_linear_regression_parameters(file_name);
}


void TestingAnalysisTest::test_print_linear_regression_analysis(void)
{
   message += "test_print_linear_regression_analysis\n";
}


void TestingAnalysisTest::test_save_linear_regression_analysis(void)
{
   message += "test_save_linear_regression_analysis\n";
}


void TestingAnalysisTest::test_calculate_confusion(void)
{
   message += "test_calculate_confusion\n";

   NeuralNetwork nn;
   DataSet ds;

   TestingAnalysis ta(&nn, &ds);

  // Instances* i;

   Matrix<double> actual;
   Matrix<double> predicted;

   Matrix<size_t> confusion;

   // Test

   actual.set(4, 3);
   predicted.set(4, 3);

   actual(0,0) = 1; actual(0,1) = 0; actual(0,2) = 0;
   actual(1,0) = 0; actual(1,1) = 1; actual(1,2) = 0;
   actual(2,0) = 0; actual(2,1) = 1; actual(2,2) = 0;
   actual(3,0) = 0; actual(3,1) = 0; actual(3,2) = 1;

   predicted(0,0) = 1; predicted(0,1) = 0; predicted(0,2) = 0;
   predicted(1,0) = 0; predicted(1,1) = 1; predicted(1,2) = 0;
   predicted(2,0) = 0; predicted(2,1) = 1; predicted(2,2) = 0;
   predicted(3,0) = 0; predicted(3,1) = 0; predicted(3,2) = 1;

   confusion = ta.calculate_confusion_multiple_classification(actual, predicted);

   assert_true(confusion.calculate_sum() == 4, LOG);
   assert_true(confusion.get_diagonal().calculate_sum() == 4, LOG);
}


void TestingAnalysisTest::test_calculate_binary_classification_test(void)
{
   message += "test_calculate_binary_classification_test\n";
}


void TestingAnalysisTest::test_calculate_Wilcoxon_parameter(void)
{
    message += "test_calculate_Wilcoxon_parameter\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    double wilcoxon_parameter;

    // Test

    double x = 1.5;
    double y = 2.5;

    wilcoxon_parameter = ta.calculate_Wilcoxon_parameter(x, y);

    assert_true(wilcoxon_parameter == 0, LOG);

    // Test

    wilcoxon_parameter = ta.calculate_Wilcoxon_parameter(y ,x);

    assert_true(wilcoxon_parameter == 1, LOG);

    // Test

    x = y;

    wilcoxon_parameter = ta.calculate_Wilcoxon_parameter(x, y);

    assert_true(wilcoxon_parameter == 0.5, LOG);
}


void TestingAnalysisTest::test_calculate_roc_curve(void)
{
    message += "test_calculate_roc_curve\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    Matrix <double> roc_curve;

    // Test

    target_data.set(4, 1);

    target_data(0,0) = 0.0;
    target_data(1,0) = 0.0;
    target_data(2,0) = 1.0;
    target_data(3,0) = 1.0;

    output_data.set(4, 1);

    output_data(0,0) = 0.0;
    output_data(1,0) = 0.0;
    output_data(2,0) = 1.0;
    output_data(3,0) = 1.0;

    roc_curve = ta.calculate_roc_curve(target_data, output_data);

    assert_true(roc_curve(0, 0) == 0, LOG);
    assert_true(roc_curve(0, 1) == 0, LOG);
    assert_true(roc_curve(1, 0) == 0, LOG);
    assert_true(roc_curve(1, 1) == 0, LOG);
    assert_true(roc_curve(2, 0) == 0, LOG);
    assert_true(roc_curve(2, 1) == 1, LOG);
    assert_true(roc_curve(3, 0) == 0, LOG);
    assert_true(roc_curve(3, 1) == 1, LOG);
    assert_true(roc_curve(4, 0) == 1, LOG);
    assert_true(roc_curve(4, 1) == 1, LOG);
    assert_true(roc_curve.get_columns_number() == 2, LOG);
    assert_true (roc_curve.get_rows_number() == 5, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.12;
    output_data(1, 0) = 0.78;
    output_data(2, 0) = 0.84;
    output_data(3, 0) = 0.99;

    roc_curve = ta.calculate_roc_curve(target_data, output_data);

    assert_true(roc_curve(0, 0) == 0, LOG);
    assert_true(roc_curve(0, 1) == 0, LOG);
    assert_true(roc_curve(1, 0) == 0, LOG);
    assert_true(roc_curve(1, 1) == 0.5, LOG);
    assert_true(roc_curve(2, 0) == 0.5, LOG);
    assert_true(roc_curve(2, 1) == 0.5, LOG);
    assert_true(roc_curve(3, 0) == 1, LOG);
    assert_true(roc_curve(3, 1) == 0.5, LOG);
    assert_true(roc_curve(4, 0) == 1, LOG);
    assert_true(roc_curve(4, 1) == 1, LOG);
    assert_true(roc_curve.get_columns_number() == 2, LOG);
    assert_true(roc_curve.get_rows_number() == 5, LOG);
}


void TestingAnalysisTest::test_calculate_area_under_curve(void)
{
    message += "test_calculate_area_under_curve\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    double area_under_curve;

    // Test

    target_data.set(4, 1);

    target_data(0,0) = 0.0;
    target_data(1,0) = 0.0;
    target_data(2,0) = 1.0;
    target_data(3,0) = 1.0;

    output_data.set(4, 1);

    output_data(0,0) = 0.0;
    output_data(1,0) = 0.0;
    output_data(2,0) = 1.0;
    output_data(3,0) = 1.0;

    area_under_curve = ta.calculate_area_under_curve(target_data, output_data);

    assert_true(area_under_curve == 1, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 1.0;

    area_under_curve = ta.calculate_area_under_curve(target_data, output_data);

    assert_true(area_under_curve == 0.5, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.78;
    output_data(1, 0) = 0.84;
    output_data(2, 0) = 0.12;
    output_data(3, 0) = 0.99;

    area_under_curve = ta.calculate_area_under_curve(target_data, output_data);

    assert_true(area_under_curve == 0.5, LOG);

    // Test

    target_data.set(4,1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 0.0;

    area_under_curve = ta.calculate_area_under_curve(target_data, output_data);

    assert_true(area_under_curve == 0, LOG);

}


void TestingAnalysisTest::test_calculate_optimal_threshold(void)
{
    message += "test_calculate_optimal_threshold\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    double optimal_threshold;

    // Test

    target_data.set(4,1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 0.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    optimal_threshold = ta.calculate_optimal_threshold(target_data, output_data);

    assert_true(optimal_threshold == 1.0, LOG);

    // Test

    target_data.set(4,1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 0.0;

    optimal_threshold = ta.calculate_optimal_threshold(target_data, output_data);

    assert_true(optimal_threshold == 1.0, LOG);

    // Test

    target_data.set(5,1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 1.0;
    target_data(4, 0) = 0.0;

    output_data.set(5, 1);

    output_data(0, 0) = 0.33;
    output_data(1, 0) = 0.14;
    output_data(2, 0) = 0.12;
    output_data(3, 0) = 0.62;
    output_data(4, 0) = 0.85;

    optimal_threshold = ta.calculate_optimal_threshold(target_data, output_data);

    assert_true(optimal_threshold == 0.62, LOG);
}


void TestingAnalysisTest::test_calculate_cumulative_gain(void)
{
    message += "test_calculate_cumulative_chart\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    Matrix <double> cumulative_gain;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.67;
    output_data(1, 0) = 0.98;
    output_data(2, 0) = 0.78;
    output_data(3, 0) = 0.45;

    cumulative_gain = ta.calculate_cumulative_gain(target_data, output_data);

    assert_true(cumulative_gain.get_columns_number() == 2, LOG);
    assert_true(cumulative_gain.get_rows_number() == 21, LOG);
    assert_true(cumulative_gain(0, 0) == 0.0, LOG);
    assert_true(cumulative_gain(0, 1) == 0.0, LOG);
    assert_true(cumulative_gain(20, 0) - 1.0 < 1.0e-6, LOG);
    assert_true(cumulative_gain(20, 1) == 1.0, LOG);
 }


void TestingAnalysisTest::test_calculate_lift_chart(void)
{
    message += "test_calculate_lift_chart\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    Matrix <double> cumulative_gain;

    Matrix <double> lift_chart;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.67;
    output_data(1, 0) = 0.87;
    output_data(2, 0) = 0.99;
    output_data(3, 0) = 0.88;

    cumulative_gain = ta.calculate_cumulative_gain(target_data, output_data);

    lift_chart = ta.calculate_lift_chart(cumulative_gain);

    assert_true(lift_chart.get_columns_number() == cumulative_gain.get_columns_number(), LOG);
    assert_true(lift_chart.get_rows_number() == cumulative_gain.get_rows_number(), LOG);
}


void TestingAnalysisTest::test_calculate_calibration_plot(void)
{
    message += "test_calculate_calibration_plot\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    Matrix <double> calibration_plot;

    // Test

    target_data.set(10, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;
    target_data(4, 0) = 1.0;
    target_data(5, 0) = 1.0;
    target_data(6, 0) = 1.0;
    target_data(7, 0) = 0.0;
    target_data(8, 0) = 1.0;
    target_data(9, 0) = 0.0;

    output_data.set(10, 1);

    output_data(0, 0) = 0.09;
    output_data(1, 0) = 0.19;
    output_data(2, 0) = 0.29;
    output_data(3, 0) = 0.39;
    output_data(4, 0) = 0.49;
    output_data(5, 0) = 0.59;
    output_data(6, 0) = 0.58;
    output_data(7, 0) = 0.79;
    output_data(8, 0) = 0.89;
    output_data(9, 0) = 0.99;

    calibration_plot = ta.calculate_calibration_plot(target_data, output_data);

    assert_true(calibration_plot.get_columns_number() == 2, LOG);
    assert_true(calibration_plot.get_rows_number() == 11, LOG);
}


void TestingAnalysisTest::test_calculate_true_positive_instances(void)
{
    message += "test_calculate_true_positive_instances\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix <double> target_data;
    Matrix <double> output_data;

    Vector<size_t> true_positive_instances;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 0.0;

    const Vector<size_t> testing_indices(0, 1, 3);
    const double threshold = 0.5;

    true_positive_instances = ta.calculate_true_positive_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_positive_instances.size() == 1, LOG);
    assert_true(true_positive_instances[0] == 1, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    true_positive_instances = ta.calculate_true_positive_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_positive_instances.empty(), LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    true_positive_instances = ta.calculate_true_positive_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_positive_instances.size() == 4, LOG);
    assert_true(true_positive_instances[0] == 0, LOG);
    assert_true(true_positive_instances[1] == 1, LOG);
    assert_true(true_positive_instances[2] == 2, LOG);
    assert_true(true_positive_instances[3] == 3, LOG);
}


void TestingAnalysisTest::test_calculate_false_positive_instances(void)
{
    message += "test_calculate_false_positive_instaces\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix<double> target_data;
    Matrix<double> output_data;

    Vector<size_t> false_positive_instances;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 0.0;

    const Vector<size_t> testing_indices(0, 1, 3);
    const double threshold = 0.5;

    false_positive_instances = ta.calculate_false_positive_instances(target_data, output_data,testing_indices, threshold);

    assert_true(false_positive_instances.size() == 1, LOG);
    assert_true(false_positive_instances[0] == 2, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    false_positive_instances = ta.calculate_false_positive_instances(target_data, output_data,testing_indices, threshold);

    assert_true(false_positive_instances.size() == 4, LOG);
    assert_true(false_positive_instances[0] == 0, LOG);
    assert_true(false_positive_instances[1] == 1, LOG);
    assert_true(false_positive_instances[2] == 2, LOG);
    assert_true(false_positive_instances[3] == 3, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 0.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    false_positive_instances = ta.calculate_false_positive_instances(target_data, output_data,testing_indices, threshold);

    assert_true(false_positive_instances.empty(), LOG);
}


void TestingAnalysisTest::test_calculate_false_negative_instances(void)
{
    message += "test_calculate_false_negative_instances\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix<double> target_data;
    Matrix<double> output_data;

    Vector<size_t> false_negative_instances;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 0.0;

    const Vector<size_t> testing_indices(0, 1, 3);
    const double threshold = 0.5;

    false_negative_instances = ta.calculate_false_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(false_negative_instances.size() == 1, LOG);
    assert_true(false_negative_instances[0] == 3, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 1.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 0.0;

    false_negative_instances = ta.calculate_false_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(false_negative_instances.empty(), LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 1.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 1.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 0.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 0.0;

    false_negative_instances = ta.calculate_false_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(false_negative_instances.size() == 4, LOG);
    assert_true(false_negative_instances[0] == 0, LOG);
    assert_true(false_negative_instances[1] == 1, LOG);
    assert_true(false_negative_instances[2] == 2, LOG);
    assert_true(false_negative_instances[3] == 3, LOG);
}


void TestingAnalysisTest::test_calculate_true_negative_instances(void)
{
    message += "test_calculate_true_negative_instances\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix<double> target_data;
    Matrix<double> output_data;

    Vector<size_t> true_negative_instances;

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 0.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 0.0;
    output_data(2, 0) = 0.0;
    output_data(3, 0) = 0.0;

    const Vector<size_t> testing_indices(0, 1, 3);
    const double threshold = 0.5;

    true_negative_instances = ta.calculate_true_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_negative_instances.size() == 4, LOG);
    assert_true(true_negative_instances[0] == 0, LOG);
    assert_true(true_negative_instances[1] == 1, LOG);
    assert_true(true_negative_instances[2] == 2, LOG);
    assert_true(true_negative_instances[3] == 3, LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 1.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    true_negative_instances = ta.calculate_true_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_negative_instances.empty(), LOG);

    // Test

    target_data.set(4, 1);

    target_data(0, 0) = 0.0;
    target_data(1, 0) = 0.0;
    target_data(2, 0) = 1.0;
    target_data(3, 0) = 0.0;

    output_data.set(4, 1);

    output_data(0, 0) = 0.0;
    output_data(1, 0) = 1.0;
    output_data(2, 0) = 1.0;
    output_data(3, 0) = 1.0;

    true_negative_instances = ta.calculate_true_negative_instances(target_data, output_data, testing_indices, threshold);

    assert_true(true_negative_instances.size() == 1, LOG);
    assert_true(true_negative_instances[0] == 0, LOG);
}


void TestingAnalysisTest::test_calculate_multiple_classification_rates(void)
{
    message += "test_calculate_multiple_classification_rates\n";

    NeuralNetwork nn;
    DataSet ds;

    TestingAnalysis ta(&nn, &ds);

    Matrix<double> target_data;
    Matrix<double> output_data;

    // Test

    target_data.set(9, 3);
    output_data.set(9, 3);

    target_data(0,0) = 1; target_data(0,1) = 0; target_data(0,2) = 0;
    target_data(1,0) = 0; target_data(1,1) = 1; target_data(1,2) = 0;
    target_data(2,0) = 0; target_data(2,1) = 0; target_data(2,2) = 1;
    target_data(3,0) = 1; target_data(3,1) = 0; target_data(3,2) = 0;
    target_data(4,0) = 0; target_data(4,1) = 1; target_data(4,2) = 0;
    target_data(5,0) = 0; target_data(5,1) = 0; target_data(5,2) = 1;
    target_data(6,0) = 1; target_data(6,1) = 0; target_data(6,2) = 0;
    target_data(7,0) = 0; target_data(7,1) = 1; target_data(7,2) = 0;
    target_data(8,0) = 0; target_data(8,1) = 0; target_data(8,2) = 1;

    output_data(0,0) = 1; output_data(0,1) = 0; output_data(0,2) = 0;
    output_data(1,0) = 0; output_data(1,1) = 1; output_data(1,2) = 0;
    output_data(2,0) = 0; output_data(2,1) = 0; output_data(2,2) = 1;
    output_data(3,0) = 0; output_data(3,1) = 1; output_data(3,2) = 0;
    output_data(4,0) = 1; output_data(4,1) = 0; output_data(4,2) = 0;
    output_data(5,0) = 0; output_data(5,1) = 1; output_data(5,2) = 0;
    output_data(6,0) = 0; output_data(6,1) = 0; output_data(6,2) = 1;
    output_data(7,0) = 0; output_data(7,0) = 0; output_data(7,2) = 1;
    output_data(8,0) = 1; output_data(8,1) = 0; output_data(8,2) = 0;

    const Vector<size_t> testing_indices(0, 1, 8);

    Matrix< Vector<size_t> > multiple_classification_rates = ta.calculate_multiple_classification_rates(target_data, output_data, testing_indices);

    assert_true(multiple_classification_rates(0,0).size() == 1, LOG);
    assert_true(multiple_classification_rates(0,0)[0] == 0, LOG);

    assert_true(multiple_classification_rates(0,1).size() == 1, LOG);
    assert_true(multiple_classification_rates(0,1)[0] == 3, LOG);

    assert_true(multiple_classification_rates(0,2).size() == 1, LOG);
    assert_true(multiple_classification_rates(0,2)[0] == 6, LOG);

    assert_true(multiple_classification_rates(1,0).size() == 1, LOG);
    assert_true(multiple_classification_rates(1,0)[0] == 4, LOG);

    assert_true(multiple_classification_rates(1,1).size() == 1, LOG);
    assert_true(multiple_classification_rates(1,1)[0] == 1, LOG);

    assert_true(multiple_classification_rates(1,2).size() == 1, LOG);
    assert_true(multiple_classification_rates(1,2)[0] == 7, LOG);

    assert_true(multiple_classification_rates(2,0).size() == 1, LOG);
    assert_true(multiple_classification_rates(2,0)[0] == 8, LOG);

    assert_true(multiple_classification_rates(2,1).size() == 1, LOG);
    assert_true(multiple_classification_rates(2,1)[0] == 5, LOG);

    assert_true(multiple_classification_rates(2,2).size() == 1, LOG);
    assert_true(multiple_classification_rates(2,2)[0] == 2, LOG);
}


void TestingAnalysisTest::run_test_case(void)
{
   message += "Running testing analysis test case...\n";

   // Constructor and destructor methods

   test_constructor();
   test_destructor();

   // Get methods
   
   test_get_neural_network_pointer();
   test_get_data_set_pointer();
   
   test_get_display();

   // Set methods

   test_set_neural_network_pointer();
   test_set_data_set_pointer();

   test_set_display();

   // Target and output data methods

   test_calculate_target_output_data();

   // Error data methods

   test_calculate_error_data();

   test_calculate_error_data_statistics();
   test_calculate_error_data_statistics_matrices();

   test_calculate_error_data_histograms();

   // Linear regression analysis methods

   test_calculate_linear_regression_parameters();
   test_print_linear_regression_parameters();
   test_save_linear_regression_parameters();

   test_print_linear_regression_analysis();
   test_save_linear_regression_analysis();


   // Binary classification test methods

   test_calculate_binary_classification_test();

   // Confusion matrix methods

   test_calculate_confusion();

   // ROC curve methods

   test_calculate_Wilcoxon_parameter();

   test_calculate_roc_curve();
   test_calculate_area_under_curve();
   test_calculate_optimal_threshold();

   // Lift chart methods

   test_calculate_cumulative_gain();
   test_calculate_lift_chart();

   // Calibration plot

   test_calculate_calibration_plot();

   // Binary classification rates

   test_calculate_true_positive_instances();
   test_calculate_false_positive_instances();
   test_calculate_false_negative_instances();
   test_calculate_true_negative_instances();

   // Multiple classification rates

   test_calculate_multiple_classification_rates();

   message += "End of testing analysis test case.\n";
}


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2016 Roberto Lopez.
//
// This library is free software; you can redistribute it and/or
// modify it under the s of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
