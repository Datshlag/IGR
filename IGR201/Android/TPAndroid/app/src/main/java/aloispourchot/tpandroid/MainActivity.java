package aloispourchot.tpandroid;

import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Hashtable;
import java.util.Iterator;

public class MainActivity extends AppCompatActivity {

    String baseURLString = "https://openexchangerates.org/api/latest.json";
    GetCurrenciesRatesTask getCurrenciesRatesTask;
    ProgressDialog dialog;
    Hashtable<String, Double> currenciesRates;
    double amount;
    double rate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        startGetCurrenciesRatesTask();
    }

    /**
     * To update the ui
     */
    public void updateResult(){

        System.out.println("update result called");
        EditText ratetext = (EditText) findViewById(R.id.rate_value);
        EditText result = (EditText) findViewById(R.id.result_value);
        ratetext.setText(String.valueOf(rate));
        result.setText(String.valueOf(rate*amount));
    }

    /**
     * Starts the async task that will get the rates and currencies
     */
    private void startGetCurrenciesRatesTask() {
        dialog = ProgressDialog.show(MainActivity.this, "",
                "Loading. Please wait...", true);
        getCurrenciesRatesTask = new GetCurrenciesRatesTask();
        getCurrenciesRatesTask.execute((Void) null);
    }

    /**
     * Initializes the spinner that allows to choose the destination currency
     */
    private void initializeSpinner(Hashtable<String, Double> data) {
        dialog.dismiss();

        ArrayList<String> currenciesList = new ArrayList<String>();
        for (String str :data.keySet())
            currenciesList.add(str);
        Collections.sort(currenciesList);
        final ArrayList<Double> ratesList = new ArrayList<Double>();

        for(int i=0; i<currenciesList.size(); i++)
        {

            ratesList.add(data.get(currenciesList.get(i)));
        }

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(
                this, R.layout.row, currenciesList);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        Spinner spinner = (Spinner) findViewById(R.id.spinner);
        spinner.setAdapter(adapter);

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

                rate = ratesList.get(position);
                updateResult();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                rate = 1;
                updateResult();
            }
        });

        EditText in = (EditText) findViewById(R.id.edit_euros);
        in.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {

                amount = Float.parseFloat(s.toString());
                updateResult();
            }
        });
    }

    /**
     * Part of the code that takes care of creating a simple http client and sending a GET request at the openexchange API
     */
    private JSONObject httpGetCall(String urlString, String jsonArrayName) {
        JSONObject result = null;
        HttpClient client = new DefaultHttpClient();
        HttpGet request = new HttpGet(urlString);
        try {
            HttpResponse getResponse = client.execute(request);
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(
                            getResponse.getEntity().getContent(), "UTF-8"));
            StringBuilder builder = new StringBuilder();
            for (String line = null; (line = reader.readLine()) != null;) {
                builder.append(line).append("\n");
            }
            result = (new JSONObject(builder.toString())).getJSONObject(jsonArrayName);
            System.out.println(result.length());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }

    public class GetCurrenciesRatesTask extends AsyncTask<Void, Void, JSONObject> {
        String urlString = baseURLString + "?app_id=696c8c86625d486589ddf352fc723460";

        @Override
        protected JSONObject doInBackground(Void... arg0) {
            return httpGetCall(urlString, "rates");
        }
        @Override
        protected void onPostExecute(JSONObject getCurrenciesRatesResult) {
            currenciesRates = new Hashtable<String, Double>();
            Iterator<String> iterator = getCurrenciesRatesResult.keys();
            while (iterator.hasNext()) {
                String key = iterator.next();
                /* Sometimes the value is a Double and sometimes it's a Integer which isn't very convenient since we can't cast one of those
                * Class into the other.
                 */
                try {
                    double x;
                    try{
                        x=(Double) getCurrenciesRatesResult.get(key);
                    }
                    catch(java.lang.ClassCastException e)
                    {
                        x=((Integer) getCurrenciesRatesResult.get(key)).floatValue();
                    }
                    currenciesRates.put(key, x);
                } catch (JSONException e) {
                }
            }
            initializeSpinner(currenciesRates);
        }
    }

}
