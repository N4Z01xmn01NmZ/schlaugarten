package com.example.uas

import android.annotation.SuppressLint
import android.app.NotificationManager
import android.content.ContentValues.TAG
import android.content.Intent
import android.content.SharedPreferences
import android.os.Bundle
import android.os.Handler
import android.renderscript.Sampler
import android.service.autofill.OnClickAction
import android.text.TextUtils
import android.text.style.ClickableSpan
import android.util.Log
import android.util.TypedValue
import android.view.LayoutInflater
import android.view.View
import android.widget.Button
import android.widget.CompoundButton
import android.widget.TextView
import android.widget.ToggleButton
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat
import androidx.core.text.parseAsHtml
import frags.BottomSheetFragment
import com.google.android.material.bottomsheet.BottomSheetDialog
import com.google.android.material.snackbar.Snackbar
import com.google.firebase.FirebaseOptions
import com.google.firebase.database.*
import com.google.firebase.database.ktx.FirebaseDatabaseKtxRegistrar
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase
import kotlinx.android.synthetic.main.activity_main.*
import java.lang.StringBuilder

class MainActivity : AppCompatActivity() {
    private lateinit var  toggle2:SwitchCompat
    private lateinit var toggle1:SwitchCompat
    private lateinit var Humidity:TextView
    private lateinit var Takaran:TextView
    private lateinit var Temprature:TextView
    private lateinit var toggle3:ToggleButton
    private var progr  = 20
    private var progr2: Int = 60
    private var progr3: Int = 70


    @SuppressLint("UseCompatLoadingForDrawables")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val database = Firebase.database
        val myRef = database.getReference("device/soilIsDry")
        val myref2 = database.getReference("device/water/flowrateMillis")
        val myref3 = database.getReference("device/temperature")
        val myreflampu = database.getReference("device/lamp")
        val myrefair = database.getReference("device/valve")

        Humidity = findViewById(R.id.nilai1)
        Takaran = findViewById(R.id.nilai2)
        Temprature = findViewById(R.id.nilai3)
        toggle1 = findViewById(R.id.led_switch)
        toggle2 = findViewById(R.id.water_switch)
        toggle3 = findViewById(R.id.manual_switch)



       myRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // Get Post object and use the values to update the UI
                val value = dataSnapshot.getValue().toString()
                val valueval = value.toFloat()
                val valpercentage = valueval*100
                val kelembaban = valpercentage.toString()
                Humidity.setText(kelembaban)
                progr = valpercentage.toInt()
                updateProgressBar()
            }

            override fun onCancelled(databaseError: DatabaseError) {
              // Getting Post failed, log a message
               Log.w(TAG, "loadPost:onCancelled", databaseError.toException())
           }
        })

        myref2.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // Get Post object and use the values to update the UI
                val value = dataSnapshot.getValue().toString()
                val valueval = value.toInt()
                val valpercentage = valueval/10
                val TakaranAir = valpercentage.toString()
                Takaran.setText(value)
                progr2 = valpercentage.toInt()
                updateProgressBar2()
            }

            override fun onCancelled(databaseError: DatabaseError) {
                // Getting Post failed, log a message
                Log.w(TAG, "loadPost:onCancelled", databaseError.toException())
            }
        })

        myref3.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // Get Post object and use the values to update the UI
                val value = dataSnapshot.getValue().toString()
                val valuetemp = value.toInt()
                Temprature.setText(value)
                progr3 = valuetemp
                updateProgressBar3()
            }

            override fun onCancelled(databaseError: DatabaseError) {
                // Getting Post failed, log a message
                Log.w(TAG, "loadPost:onCancelled", databaseError.toException())
            }
        })

        toggle1.setOnCheckedChangeListener { buttonView, isChecked ->
            if (isChecked){
                myreflampu.setValue(true)
        }
            else{
                myreflampu.setValue(false)
        } }

        toggle2.setOnCheckedChangeListener { buttonView, isChecked ->
            val  angka = progr
            if (isChecked){
                if(angka < 70){
                    myrefair.setValue(true) }
                else {
                    myrefair.setValue(false)
                }
            }
            else {
                myrefair.setValue(false)
            }
        }
        toggle3.setOnCheckedChangeListener { buttonView, isChecked ->
            if(isChecked){
                myrefair.setValue(true)
            }
            else
                myrefair.setValue(false)
        }
        

            op50ml.setOnClickListener {
            val bottomSheetFragment = BottomSheetFragment(this)
            bottomSheetFragment.show(supportFragmentManager, bottomSheetFragment.tag)
        }



    }
    private fun updateProgressBar() {
        progress_bar.progress = progr
        nilai1.text = "$progr RH"
    }
    private fun updateProgressBar2() {
        progress_bar2.progress = progr2
        nilai2.text = "$progr2 dL/Min"
    }
    private fun updateProgressBar3(){
        progress_bar3.progress = progr3
        nilai3.text = "$progr3°C"
    }
}



