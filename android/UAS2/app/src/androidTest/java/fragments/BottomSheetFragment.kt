package fragments

import android.app.Activity
import android.app.TimePickerDialog
import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.media.RingtoneManager
import android.net.Uri
import android.os.Bundle
import android.text.TextUtils
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import com.google.android.material.bottomsheet.BottomSheetDialogFragment
import com.example.uas.MainActivity
import com.example.uas.R
import kotlinx.android.synthetic.main.bottom_sheet_fragment.*
import java.math.RoundingMode
import java.text.DecimalFormat
import java.util.*


class BottomSheetFragment: BottomSheetDialogFragment() {

    private lateinit var sharedPref: SharedPreferences
    private var weight: String = ""
    private var workTime: String = ""
    private var customTarget: String = ""
    private var wakeupTime: Long = 0
    private var sleepingTime: Long = 0
    private var notificMsg: String = ""
    private var notificFrequency: Int = 0
    private var currentToneUri: String? = ""

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.bottom_sheet_fragment, container, false)

    }
}