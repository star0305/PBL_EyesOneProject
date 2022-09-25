package com.example.pbl_practice

import android.os.Build
import android.os.SystemClock
import androidx.annotation.RequiresApi
import java.time.LocalDateTime
import java.time.LocalDateTime.now
import java.time.format.DateTimeFormatter.ofPattern
import java.util.*


@RequiresApi(Build.VERSION_CODES.O)
fun main() {
     var count = 0
     var time1 : Long = 0
     var time2 : Long = 0
     var interval : Int = 0
     var signal = 0



     var trycount = 0

     while (true) {

          println("signal 입력: \n")
          val DateAndTime: LocalDateTime = now()
          val formatter = ofPattern("yyyy년 mm월 dd일 HH시 mm분 ss초")
          val formatted = DateAndTime.format(formatter)
          signal = Scanner(System.`in`).nextInt()

          when (signal) {
               1 -> {

                    println("도어락이 해제되었습니다. 현재시간 $formatted")
               }
               2 -> {
                    trycount = trycount +1
                    if (trycount == 1) {
                         time1 = System.currentTimeMillis()
                         println("잘못된 접근이 감지되었습니다. 현재시간 $formatted , 시도횟수 $trycount ")

                         //time1 첫번재 시도
                    } else {
                         time2 = System.currentTimeMillis()
                         println("잘못된 접근이 감지되었습니다. 현재시간 $formatted , 시도횟수 $trycount")

                         //time2 두번째 이후의 시도에 대한 시간
                    }
               }

          }
          interval = ((time2 - time1)/1000).toInt()

          if(interval > 30){
               trycount = 0
               time1 = 0
               time2 = 0
          }
          if(interval > 0)
               println("Interval time is $interval seconds.")
          if(trycount>30){
               println("잘못된 시도 횟수가 초과 되었습니다. 키패드를 통해 잠금해제 하십시오 .")
               break
          }

     }
}
