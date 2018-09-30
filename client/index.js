/// <reference path="jquery.d.ts" />
/// <reference path="knockout.d.ts" />
/**
 * General purpose AJAX exchange function that sends and receives JSON objects.
 */
function sendAjaxPost(postData, callback) {
    $.ajax({
        type: 'POST',
        url: 'json-rpc',
        data: JSON.stringify(postData),
        dataType: 'json',
        async: true,
        error: function (jqXHR, status) {
            // Failure.
            callback(false, { 'error': 'AJAX Error: ' + status });
        },
        success: function (data, status, jqXHR) {
            // Success, although the returned object could have 'error'.
            callback(true, jqXHR.responseJSON);
        }
    });
}
/******************************************************************************
 *
 * View model.
 *
 ******************************************************************************/
var CalcData = /** @class */ (function () {
    function CalcData() {
        this.fcn = 'calculate';
        this.arg = [1, 2, 3];
    }
    return CalcData;
}());
var ExampleViewModel = /** @class */ (function () {
    function ExampleViewModel() {
        this.value1 = ko.observable('1');
        this.value2 = ko.observable('2');
        this.value3 = ko.observable('3');
        this.result = ko.observable('6');
        ko.applyBindings(this, $('body')[0]);
    }
    ExampleViewModel.prototype.onClick = function () {
        var self = this;
        var calcData;
        // Create a new CalcData object.
        calcData = new CalcData();
        // Convert the edit boxes to numbers.
        calcData.arg[0] = Number(self.value1());
        calcData.arg[1] = Number(self.value2());
        calcData.arg[2] = Number(self.value3());
        // Make the AJAX request.
        sendAjaxPost(calcData, function (isSuccess, obj) {
            /*
             * The response will be either:
             *
             * {'result': number}		// Success.
             * 		or
             * {'error' : 'reason'}		// Error; networ/client/server
             *
             */
            // Ignore errors for now.
            if (!isSuccess || (obj['error'] !== undefined))
                return;
            // Display the result.
            self.result(String(obj['result']));
        });
    };
    return ExampleViewModel;
}());
//
var exampleViewModel;
/******************************************************************************
 *
 * Main
 *
 ******************************************************************************/
$(document).ready(function () {
    window.resizeTo(800, 500);
    exampleViewModel = new ExampleViewModel();
});
//# sourceMappingURL=index.js.map