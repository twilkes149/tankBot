import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {

  constructor(public navCtrl: NavController) {

  }

  forward() {
    console.log("forward");
  }

  left() {
    console.log("left");
  }

  right() {
    console.log("right");
  }

  reverse() {
    console.log("reverse");
  }

  stop() {
    console.log("stop");
  }

}
